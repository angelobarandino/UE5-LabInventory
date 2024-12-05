// Fill out your copyright notice in the Description page of Project Settings.


#include "LabInventoryComponent.h"

#include "LabInventory/LabInventory.h"
#include "LabInventory/Core/LabInventoryItemInstance.h"
#include "LabInventory/Items/LabInventoryItem.h"
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
		if (Item->Instance.InventoryItem.IsValid())
		{
			InventoryItem = Item->Instance;
			return true;
		}
	}
	
	return false;
}

FLabAddItemParams ULabInventoryComponent::FindInventorySlotForItem(const int32 ItemCount, const TSoftObjectPtr<ULabInventoryItem>& InventoryItem)
{
	FLabAddItemParams Params;
	Params.Status = InventoryFull;
	Params.InventoryItem = InventoryItem;

	if (!InventoryItem.IsValid())
	{
		UE_LOG(LogInventory, Warning, TEXT("FindInventorySlotForItem called with an invalid InventoryItem."));
		Params.Status = InventoryItemInvalid;
		return Params;
	}
	
	// Retrieve item stacking information (stackable and max stack size)
	RetrieveItemStackingInfo(InventoryItem.Get(), Params.bStackable, Params.StackSize);

	// Search for an available slot
	for (int32 SlotIndex = 0; SlotIndex < ItemsSize; ++SlotIndex)
	{
		const FLabInventoryEntry* ItemEntry = InventoryList.GetItemAtSlot(SlotIndex);
		
		// If slot is empty, we can insert the item here
		if (ItemEntry == nullptr)
		{
			Params.SlotIndex = SlotIndex;
			Params.Status = InsertToSlot;
			Params.SlotCurrentItemCount = 0;
			break;
		}
		
		// If the item in the slot is the same type as the requested item
		TSoftObjectPtr<ULabInventoryItem> InstanceInventoryItem = ItemEntry->Instance.InventoryItem;
		if (InstanceInventoryItem.IsValid() && InstanceInventoryItem.Get() == InventoryItem.Get())
		{
			// If the item is stackable and the slot is full, continue to the next slot
			if(Params.bStackable)
			{
				if (Params.StackSize == ItemEntry->Instance.ItemCount)
				{
                    // Stack is already full, continue to the next slot
					continue;
				}
				
				// If stackable, but there is room for more, update the status to update slot
				Params.Status = UpdateItemSlot;
				Params.SlotIndex = SlotIndex;
				Params.SlotCurrentItemCount = ItemEntry->Instance.ItemCount;
				break;
			}
		}
	}
	
	// Calculate the max available items that can be added to the found slot
	Params.SlotMaxAvailableItems = CalculateMaxItemsToAdd(Params.StackSize, Params.SlotCurrentItemCount, ItemCount);
	
	return Params;
}

bool ULabInventoryComponent::AddInventoryItem(const FLabAddItemParams& Params)
{
	const AActor* Owner = GetOwner();
	if (Owner->HasAuthority())
	{
		bool bResult = false;
		
		if (Params.Status == InsertToSlot)
		{
			bResult = InventoryList.AddItem(Params.SlotIndex, Params.SlotMaxAvailableItems, Params.InventoryItem);
		}

		if (Params.Status == UpdateItemSlot)
		{
			bResult = InventoryList.AddItemCount(Params.SlotIndex, Params.SlotMaxAvailableItems);
		}

		if (bResult)
		{
			OnInventoryItemUpdated.Broadcast(InventoryList.GetItemAtSlot(Params.SlotIndex)->Instance);
		}

		return bResult;
	}

	return false;
}

void ULabInventoryComponent::RetrieveItemStackingInfo(const ULabInventoryItem* InventoryItem, bool& bOutStackable, int32& OutStackSize) const
{
	// Default to 1 stack size and non-stackable if no fragment is found
	OutStackSize = 1;
	bOutStackable = false;
	
	// Attempt to find the item definition fragment
	if (const ULAB_InventoryFragment* InventoryFragment = ULabInventoryStatics::FindItemDefinitionFragment<ULAB_InventoryFragment>(InventoryItem))
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