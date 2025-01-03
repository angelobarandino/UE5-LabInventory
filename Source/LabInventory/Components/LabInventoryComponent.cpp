﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "LabInventoryComponent.h"

#include "LabInventory/LabInventory.h"
#include "LabInventory/Items/LabInventoryItemInstance.h"
#include "LabInventory/Items/LabItem.h"
#include "LabInventory/Items/LabItemFragment.h"
#include "LabInventory/Library/LabInventoryStatics.h"
#include "Net/UnrealNetwork.h"

ULabInventoryComponent::ULabInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void ULabInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
	DOREPLIFETIME(ThisClass, ItemsSize);
}

bool ULabInventoryComponent::TryGetInventoryItemAtSlot(const int32 SlotIndex, FLabInventoryItemInstance& InventoryItem)
{
	if (const FLabInventoryEntry* Item = InventoryList.GetItemAtSlot(SlotIndex))
	{
		if (Item->Instance.Item.IsValid())
		{
			InventoryItem = Item->Instance;
			return true;
		}
	}
	
	return false;
}

FLabUpdateInventoryParam ULabInventoryComponent::FindInventorySlotForItem(const int32 ItemCount, const TSoftObjectPtr<ULabItem>& InventoryItem)
{
	FLabUpdateInventoryParam UpdateParams;
	UpdateParams.Status = InventoryFull;
	UpdateParams.InventoryItem = InventoryItem;

	if (!InventoryItem.IsValid())
	{
		UE_LOG(LogLabInventory, Warning, TEXT("FindInventorySlotForItem called with an invalid InventoryItem."));
		UpdateParams.Status = InventoryItemInvalid;
		return UpdateParams;
	}
	
	// Retrieve item stacking information (stackable and max stack size)
	RetrieveItemStackingInfo(InventoryItem.Get(), UpdateParams.bStackable, UpdateParams.StackSize);

	// Search for an available slot
	for (int32 SlotIndex = 0; SlotIndex < ItemsSize; ++SlotIndex)
	{
		const FLabInventoryEntry* ItemEntry = InventoryList.GetItemAtSlot(SlotIndex);
		
		// If slot is empty, we can insert the item here
		if (ItemEntry == nullptr)
		{
			UpdateParams.SlotIndex = SlotIndex;
			UpdateParams.Status = InsertToSlot;
			UpdateParams.SlotCurrentItemCount = 0;
			break;
		}
		
		// If the item in the slot is the same type as the requested item
		if (IsItemCompatible(*ItemEntry, InventoryItem.Get()))
		{
			// If the item is stackable and the slot is full, continue to the next slot
			if(UpdateParams.bStackable)
			{
				if (UpdateParams.StackSize == ItemEntry->Instance.ItemCount)
				{
                    // Stack is already full, continue to the next slot
					continue;
				}
				
				// If stackable, but there is room for more, update the status to update slot
				UpdateParams.Status = UpdateItemSlot;
				UpdateParams.SlotIndex = SlotIndex;
				UpdateParams.SlotCurrentItemCount = ItemEntry->Instance.ItemCount;
				break;
			}
		}
	}
	
	// Calculate the max available items that can be added to the found slot
	UpdateParams.RemainingSlotCapacity = CalculateMaxItemsToAdd(UpdateParams.StackSize, UpdateParams.SlotCurrentItemCount, ItemCount);
	return UpdateParams;
}

FLabUpdateInventoryParam ULabInventoryComponent::CreateMoveToSlotForItem(const int32 SlotIndex, const int32 ItemCount, const TSoftObjectPtr<ULabItem>& InventoryItem)
{
	FLabUpdateInventoryParam UpdateParams;
	UpdateParams.SlotIndex = SlotIndex;
	UpdateParams.InventoryItem = InventoryItem;

	if (!InventoryItem.IsValid())
	{
		UE_LOG(LogLabInventory, Warning, TEXT("CreateMoveToSlotForItem called with an invalid InventoryItem."));
		UpdateParams.Status = InventoryItemInvalid;
		return UpdateParams;
	}
	
	// Retrieve item stacking information (stackable and max stack size)
	RetrieveItemStackingInfo(InventoryItem.Get(), UpdateParams.bStackable, UpdateParams.StackSize);

	if (const FLabInventoryEntry* ItemEntry = InventoryList.GetItemAtSlot(SlotIndex))
	{
		if (IsItemCompatible(*ItemEntry, InventoryItem.Get()))
		{
			// If stackable, but there is room for more, update the status to update slot
			if(UpdateParams.bStackable && UpdateParams.StackSize > ItemEntry->Instance.ItemCount)
			{
				UpdateParams.Status = UpdateItemSlot;
				UpdateParams.SlotCurrentItemCount = ItemEntry->Instance.ItemCount;
			}
			else
			{
                UE_LOG(LogLabInventory, Warning, TEXT("Slot %d is full or incompatible for stacking."), SlotIndex);
				UpdateParams.Status = UnavailableItemSlot;
			}
		}
		else
		{
            UE_LOG(LogLabInventory, Warning, TEXT("Slot %d contains a different item."), SlotIndex);
			UpdateParams.Status = UnavailableItemSlot;
		}
	}
	else
	{
        // Empty slot available.
		UpdateParams.Status =  InsertToSlot;
	}
	
	UpdateParams.RemainingSlotCapacity = CalculateMaxItemsToAdd(
		UpdateParams.StackSize, UpdateParams.SlotCurrentItemCount, ItemCount);
	
	UE_LOG(LogLabInventory, Log, TEXT("CreateMoveToSlotForItem: SlotIndex=%d, Status=%d, RemainingCapacity=%d"),
		   SlotIndex, UpdateParams.Status.GetValue(), UpdateParams.RemainingSlotCapacity);
	
	return UpdateParams;
}

bool ULabInventoryComponent::CanMoveItemToSlot(const int32 SlotIndex, const ULabItem* InventoryItem)
{
	// Ensure the InventoryItem is valid.
	if (!InventoryItem)
	{
		UE_LOG(LogLabInventory, Warning, TEXT("CanMoveItemToSlot: Invalid InventoryItem passed."));
		return false;
	}

	// Retrieve stacking info for the item.
	int32 MaxStackSize = 1;
	bool bIsStackable = true;
	RetrieveItemStackingInfo(InventoryItem, bIsStackable, MaxStackSize);

	// Check if the slot is empty.
	const FLabInventoryEntry* ItemEntry = InventoryList.GetItemAtSlot(SlotIndex);
	if (!ItemEntry)
	{
		return true; // Slot is empty, item can be moved.
	}

	// Check if the item is compatible with the existing item in the slot.
	if (IsItemCompatible(*ItemEntry, InventoryItem))
	{
		// Check stacking logic if applicable.
		if (bIsStackable && ItemEntry->Instance.ItemCount < MaxStackSize)
		{
			// Slot can accommodate more of this item.
			return true;
		}

		UE_LOG(LogLabInventory, Warning, TEXT("CanMoveItemToSlot: Slot %d cannot accommodate more of this item (stack full)."), SlotIndex);
	}
	else
	{
		UE_LOG(LogLabInventory, Warning, TEXT("CanMoveItemToSlot: Slot %d contains an incompatible item."), SlotIndex);
	}
	
	// Slot is occupied and incompatible, or stack is full.
	return false; 
}

bool ULabInventoryComponent::AddInventoryItem(const FLabUpdateInventoryParam& Params)
{
	const AActor* Owner = GetOwner();
	if (Owner->HasAuthority())
	{
		bool bResult = false;
		
		if (Params.Status == InsertToSlot)
		{
			bResult = InventoryList.AddItem(Params.SlotIndex, Params.RemainingSlotCapacity, Params.InventoryItem);
		}

		if (Params.Status == UpdateItemSlot)
		{
			bResult = InventoryList.AddItemCount(Params.SlotIndex, Params.RemainingSlotCapacity);
		}

		if (bResult)
		{
			OnInventoryItemUpdated.Broadcast(InventoryList.GetItemAtSlot(Params.SlotIndex)->Instance);
		}

		return bResult;
	}

	return false;
}

bool ULabInventoryComponent::RemoveInventoryItem(const int32 SlotIndex, const int32 ItemCount)
{
	if (InventoryList.RemoveItemCount(SlotIndex, ItemCount))
	{
		if (const FLabInventoryEntry* ItemEntry = InventoryList.GetItemAtSlot(SlotIndex))
		{
			OnInventoryItemUpdated.Broadcast(ItemEntry->Instance);
		}
		else
		{
			OnInventoryItemRemoved.Broadcast(SlotIndex);
		}
		
		return true;	
	}
	
	return false;
}

TArray<FLabInventoryItemInstance> ULabInventoryComponent::GetInventoryItems() const
{
	TArray<FLabInventoryItemInstance> InventoryItems;
	for (int Index = 0; Index < InventoryList.Items.Num(); ++Index)
	{
		const FLabInventoryEntry& Item = InventoryList.Items[Index];
		InventoryItems.Add(Item.Instance);
	}
	return InventoryItems;
}

bool ULabInventoryComponent::IsItemCompatible(const FLabInventoryEntry& ItemEntry, const ULabItem* InventoryItem) const
{
	const TSoftObjectPtr<ULabItem> InstanceInventoryItem = ItemEntry.Instance.Item;
	
	return InstanceInventoryItem.IsValid() && InstanceInventoryItem.Get() == InventoryItem;
}

void ULabInventoryComponent::RetrieveItemStackingInfo(const ULabItem* InventoryItem, bool& bOutStackable, int32& OutStackSize) const
{
	// Default to 1 stack size and non-stackable if no fragment is found
	OutStackSize = 1;
	bOutStackable = false;
	
	// Attempt to find the item definition fragment
	if (const ULabInventoryFragment* InventoryFragment = ULabInventoryStatics::FindItemDefinitionFragment<ULabInventoryFragment>(InventoryItem))
	{
        // Update stack size and stackability if a fragment is found
		OutStackSize = InventoryFragment->StackSize;
		bOutStackable = InventoryFragment->bStackable;
	}
}

int32 ULabInventoryComponent::CalculateMaxItemsToAdd(const int32 MaxStackPerSlot, const int32 CurrentItemCountInSlot, const int32 RequestedItemCount) const
{
	const int32 AvailableItemCount = MaxStackPerSlot - CurrentItemCountInSlot;

	return FMath::Max(0, FMath::Min(AvailableItemCount, RequestedItemCount));
}