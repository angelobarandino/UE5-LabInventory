#include "LabInventoryData.h"

#include "LabInventoryItemInstance.h"
#include "LabItem.h"
#include "LabInventory/Components/LabInventoryComponent.h"

void FLabInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (const int32 Index : RemovedIndices)
	{
		OwnerInventory->OnInventoryItemRemoved.Broadcast(Items[Index].Instance.SlotIndex);
	}
}

void FLabInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (const int32 Index : AddedIndices)
	{
		OwnerInventory->OnInventoryItemUpdated.Broadcast(Items[Index].Instance);
	}
}

void FLabInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (const int32 Index : ChangedIndices)
	{
		OwnerInventory->OnInventoryItemUpdated.Broadcast(Items[Index].Instance);
	}
}


FLabInventoryEntry* FLabInventoryList::GetItemAtSlot(const int32 SlotIndex)
{
	for (auto ItemIt = Items.CreateIterator(); ItemIt; ++ItemIt)
	{
		FLabInventoryEntry& Item = *ItemIt;
		
		if (Item.Instance.SlotIndex == SlotIndex && Item.Instance.Item.IsValid())
		{
			return &Item;
		}
	}

	return nullptr;
}

bool FLabInventoryList::AddItem(const int32 SlotIndex, const int32 ItemCount, const TSoftObjectPtr<ULabItem>& InventoryItem)
{
	check(InventoryItem);
	check(OwnerInventory);

	if (const AActor* OwningActor = OwnerInventory->GetOwner())
	{
		if (OwningActor->HasAuthority())
		{
			// ULAB_InventoryItemInstance* ItemInstance = NewObject<ULAB_InventoryItemInstance>(OwningActor);
			
			FLabInventoryItemInstance ItemInstance;
			ItemInstance.Item = InventoryItem;
			ItemInstance.ItemCount = ItemCount;
			ItemInstance.SlotIndex = SlotIndex;
			
			FLabInventoryEntry& NewItem = Items.AddDefaulted_GetRef();
			NewItem.InventoryComponent = OwnerInventory;
			NewItem.Instance = ItemInstance;
			
			MarkItemDirty(NewItem);
			return true;
		}
	}
	
	return false;
}

bool FLabInventoryList::AddItemCount(const int32 SlotIndex, const int32 ItemCount)
{
	check(OwnerInventory);

	if (const AActor* OwningActor = OwnerInventory->GetOwner())
	{
		if (OwningActor->HasAuthority())
		{
			if (FLabInventoryEntry* Item = GetItemAtSlot(SlotIndex))
			{
				if (Item->Instance.Item.IsValid())
				{
					Item->Instance.ItemCount += ItemCount;
					MarkItemDirty(*Item);
					return true;
				}
			}
		}
	}
	
	return false;
}

bool FLabInventoryList::RemoveItemCount(const int32 SlotIndex, const int32 ItemCount)
{
	for (auto ItemIt = Items.CreateIterator(); ItemIt; ++ItemIt)
	{
		if (ItemIt->Instance.SlotIndex == SlotIndex)
		{
			ItemIt->Instance.ItemCount -= ItemCount;

			if (ItemIt->Instance.ItemCount == 0)
			{
				ItemIt.RemoveCurrent();
				MarkArrayDirty();
				return true;
			}

			MarkItemDirty(*ItemIt);
			return true;
		}	
	}

	return false;
}
