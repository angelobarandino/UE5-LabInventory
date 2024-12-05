// Fill out your copyright notice in the Description page of Project Settings.


#include "LabInventoryGridWidget.h"

#include "LabInventorySlotWidget.h"
#include "Components/TileView.h"
#include "Engine/AssetManager.h"
#include "LabInventory/LabInventory.h"
#include "LabInventory/Components/LabInventoryComponent.h"
#include "LabInventory/Items/LabInventoryItem.h"
#include "LabInventory/Library/LabInventoryStatics.h"


ULabInventoryGridWidget::ULabInventoryGridWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULabInventoryGridWidget::InitInventory(AActor* Owner)
{
	OwnerActor = Owner;
	
	OwnerInventory = ULabInventoryStatics::GetInventoryComponent(Owner);

	if (OwnerInventory == nullptr)
	{
		UE_LOG(LogInventory, Error, TEXT("OwnerActor doesn't have attached InventoryComponent"));
		return;
	}

	CreateInventorySlots();

	OwnerInventory->OnInventoryItemUpdated.AddDynamic(this, &ThisClass::OnInventoryUpdated);
	
	OnInitializeInventory();
}

void ULabInventoryGridWidget::CreateInventorySlots() const
{
	if (OwnerInventory == nullptr)
		return;

	for (int32 SlotIndex = 0; SlotIndex < OwnerInventory->ItemsSize; SlotIndex++)
	{
		ULabInventorySlotEntry* Entry = NewObject<ULabInventorySlotEntry>();
		Entry->SlotIndex = SlotIndex;
		Entry->Inventory = OwnerInventory;

		FLabInventoryItemInstance ItemInstance;
		if (OwnerInventory->TryGetInventoryItemAtSlot(SlotIndex, ItemInstance))
		{
			Entry->ItemCount = ItemInstance.ItemCount;

			UAssetManager::GetStreamableManager().RequestAsyncLoad(
				ItemInstance.InventoryItem.ToSoftObjectPath(),
				[this, Entry, ItemInstance]()
				{
					if (ItemInstance.InventoryItem.IsValid())
					{
						Entry->InventoryItem = ItemInstance.InventoryItem.Get();
					}
					else
					{
						UE_LOG(LogInventory, Warning, TEXT("Failed to load InventoryItem at slot %d"), Entry->SlotIndex);
					}
					
					Entry->UpdateInventorySlotDisplay.Broadcast();
				}
			);
		}
		
		InventorySlots->AddItem(Entry);
	}
}

void ULabInventoryGridWidget::OnInventoryUpdated(const FLabInventoryItemInstance& Instance)
{
	if (Instance.InventoryItem.IsValid())
	{
		UObject* SlotEntry = InventorySlots->GetItemAt(Instance.SlotIndex);
		if (ULabInventorySlotEntry* ItemEntry = Cast<ULabInventorySlotEntry>(SlotEntry))
		{
			ItemEntry->ItemCount = Instance.ItemCount;
			ItemEntry->InventoryItem = Instance.InventoryItem.Get();
			ItemEntry->UpdateInventorySlotDisplay.Broadcast();
		}
	}
}
