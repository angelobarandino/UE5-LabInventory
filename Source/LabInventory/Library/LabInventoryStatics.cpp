﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "LabInventoryStatics.h"

#include "LabInventory/LabInventory.h"
#include "LabInventory/Components/LabInventoryComponent.h"
#include "LabInventory/Items/LabInventoryItemInstance.h"
#include "LabInventory/Items/LabItem.h"
#include "LabInventory/Items/LabItemFragment.h"
#include "LabInventory/Items/LabUpdateInventoryParam.h"

FLabInventoryTransactionResult ULabInventoryStatics::TryAddItemToInventory(AActor* TargetActor, const int32 ItemCount, const TSoftObjectPtr<ULabItem>& ItemDefinition)
{
	check(TargetActor);

	FLabInventoryTransactionResult TransactionResult;

    // Retrieve the inventory component from the target actor
	if (ULabInventoryComponent* InventoryComponent = GetInventoryComponent(TargetActor))
	{
		// Track how many items remain to be added
		int32 RemainingItemCount  = ItemCount;

		// Try adding items until the inventory is full or no more items can be added
		while (RemainingItemCount  > 0)
		{
			const FLabUpdateInventoryParam Params = InventoryComponent->FindInventorySlotForItem(RemainingItemCount , ItemDefinition);
			
			// If no suitable slot found (inventory full or unavailable slot), exit the loop
			if (Params.Status == InventoryFull ||
				Params.Status == UnavailableItemSlot ||
				Params.Status == InventoryItemInvalid)
			{
				break;
			}
			
			// Attempt to add the item and update the remaining count
			if (InventoryComponent->AddInventoryItem(Params))
			{
				RemainingItemCount -= Params.RemainingSlotCapacity;
			}
		}

		// Set the response result based on how many items were successfully added
		if (RemainingItemCount == 0)
		{
			TransactionResult.ChangeResult = EInventoryUpdateResult::Success;
		}
		else if (RemainingItemCount != ItemCount)
		{
			TransactionResult.ChangeResult = EInventoryUpdateResult::PartiallyAdded;
		}
		else if (RemainingItemCount == ItemCount) // If no items were added
		{
			TransactionResult.ChangeResult = EInventoryUpdateResult::FullInventory;
		}
		
		// Set the final count of items added to the inventory
		TransactionResult.ItemsAffectedCount  = ItemCount - RemainingItemCount;;
	}
	else
	{
		// Handle the case where no inventory component is found on the target actor
		TransactionResult.ChangeResult = EInventoryUpdateResult::InventoryNotFound;
	}
	
	return TransactionResult;
}

void ULabInventoryStatics::MoveInventoryItem(const FLabMoveInventoryItemParam& MoveItemParam)
{
	if (MoveItemParam.SourceInventory.IsValid() && MoveItemParam.TargetInventory.IsValid())
	{
		const int32 SourceSlotIndex = MoveItemParam.SourceSlotIndex;
		const int32 TargetSlotIndex = MoveItemParam.TargetSlotIndex;
		ULabInventoryComponent* SourceInventory = MoveItemParam.SourceInventory.Get();
		ULabInventoryComponent* TargetInventory = MoveItemParam.TargetInventory.Get();

		if (SourceSlotIndex == TargetSlotIndex && SourceInventory == TargetInventory)
		{
			return;
		}

		FLabInventoryItemInstance ItemInstance;
		if (SourceInventory->TryGetInventoryItemAtSlot(SourceSlotIndex, ItemInstance))
		{
			// check if TSoftObjectPtr<ULabInventoryItem> is valid or loaded
			if (ItemInstance.Item.IsValid())
			{
				const FLabUpdateInventoryParam Param = TargetInventory->CreateMoveToSlotForItem(
					TargetSlotIndex, ItemInstance.ItemCount, ItemInstance.Item);

				if (Param.Status == UnavailableItemSlot || Param.Status == InventoryItemInvalid)
				{
					UE_LOG(LogLabInventory, Warning, TEXT("Failed to move item. Slot unavailable or item invalid."));
					return;
				}

				TargetInventory->AddInventoryItem(Param);

				const int32 MovedItemCount = Param.RemainingSlotCapacity;
				SourceInventory->RemoveInventoryItem(SourceSlotIndex, MovedItemCount);

				UE_LOG(LogLabInventory, Log, TEXT("Moved %d items from slot %d to slot %d."), MovedItemCount, SourceSlotIndex, TargetSlotIndex);
			}
			else
			{
				UE_LOG(LogLabInventory, Warning, TEXT("Source inventory item is invalid or unloaded."));
			}
		}
		else
		{
			UE_LOG(LogLabInventory, Warning, TEXT("No item found in source inventory slot %d."), SourceSlotIndex);
		}
	}
	else
	{
		UE_LOG(LogLabInventory, Error, TEXT("Invalid source or target inventory."));
	}
}

void ULabInventoryStatics::TransferAllInventoryItems(ULabInventoryComponent* SourceInventory, ULabInventoryComponent* TargetInventory)
{
	if (SourceInventory == nullptr || TargetInventory == nullptr)
		return;
	
	if (SourceInventory == TargetInventory)
		return;

	TArray<FLabInventoryItemInstance> Items = SourceInventory->GetInventoryItems();
	for (int32 Index = 0; Index < Items.Num(); Index++)
	{
		FLabUpdateInventoryParam UpdateParam;
		
		const FLabInventoryItemInstance& ItemInstance = Items[Index];
		if (ItemInstance.Item.IsValid())
		{
			int32 RemainingItemCount = ItemInstance.ItemCount;
			while (RemainingItemCount > 0)
			{
				UpdateParam = TargetInventory->FindInventorySlotForItem(RemainingItemCount, ItemInstance.Item);
				if (!UpdateParam.CanAddItems())
					break;
				
				if (UpdateParam.InventoryItem.IsValid())
				{
					const bool bItemAdded = TargetInventory->AddInventoryItem(UpdateParam);
					if (bItemAdded)
					{
						const int32 ItemCountToRemove = UpdateParam.RemainingSlotCapacity;
						SourceInventory->RemoveInventoryItem(ItemInstance.SlotIndex, ItemCountToRemove);
						RemainingItemCount -= ItemCountToRemove;
					}
				}
			}
		}
		
		if (UpdateParam.Status == InventoryFull)
			break;
	}
}

const ULabItemFragment* ULabInventoryStatics::FindItemDefinitionFragment(const ULabItem* InventoryItem, const TSubclassOf<ULabItemFragment> FragmentClass)
{
	if (InventoryItem && FragmentClass)
	{
		for (const ULabItemFragment* Fragment : InventoryItem->Fragments)
        {
            if (Fragment->IsA(FragmentClass))
            {
            	return Fragment;
            }
        }
	}
	
	return nullptr;
}

ULabInventoryComponent* ULabInventoryStatics::GetInventoryComponent(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}
	
	ULabInventoryComponent* InventoryComponent = Actor->FindComponentByClass<ULabInventoryComponent>();
	if (InventoryComponent == nullptr)
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(Actor))
		{
			if (const APawn* Pawn = PlayerController->GetPawn())
			{
				InventoryComponent = Pawn->FindComponentByClass<ULabInventoryComponent>();
			}
		}
	}
	
	return InventoryComponent;
}

const ULabInventoryComponent* ULabInventoryStatics::GetInventoryComponentConst(AActor* Actor)
{
	return GetInventoryComponent(Actor);
}
