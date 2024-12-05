// Fill out your copyright notice in the Description page of Project Settings.


#include "LabInventoryStatics.h"

#include "LabInventory/Components/LabInventoryComponent.h"
#include "LabInventory/Core/LabAddItemParams.h"
#include "LabInventory/Items/LabInventoryItem.h"
#include "LabInventory/Items/LabItemFragment.h"

FLabInventoryTransactionResult ULabInventoryStatics::TryAddItemToInventory(AActor* TargetActor, const int32 ItemCount, const TSoftObjectPtr<ULabInventoryItem>& ItemDefinition)
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
			const FLabAddItemParams Params = InventoryComponent->FindInventorySlotForItem(RemainingItemCount , ItemDefinition);
			
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
				RemainingItemCount  -= Params.SlotMaxAvailableItems;
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

const ULabItemFragment* ULabInventoryStatics::FindItemDefinitionFragment(const ULabInventoryItem* InventoryItem, const TSubclassOf<ULabItemFragment> FragmentClass)
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
