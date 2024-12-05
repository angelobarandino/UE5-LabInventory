// Fill out your copyright notice in the Description page of Project Settings.


#include "LabInventorySlotWidget.h"

#include "LabInventoryGridWidget.h"
#include "LabInventory/Components/LabInventoryComponent.h"
#include "LabInventory/Items/LabInventoryItem.h"

void ULabInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (ListItemObject)
	{
		InventorySlotEntry = Cast<ULabInventorySlotEntry>(ListItemObject);

		ItemCount = InventorySlotEntry->ItemCount;
		InventoryItem = InventorySlotEntry->InventoryItem;
		InventorySlotEntry->UpdateInventorySlotDisplay.AddUObject(this, &ThisClass::HandleUpdateDisplay);
		
		HandleUpdateDisplay();
	}
}

void ULabInventorySlotWidget::NativeOnEntryReleased()
{
	if (InventorySlotEntry)
	{
		ItemCount = 0;
		InventoryItem = nullptr;
		InventorySlotEntry->UpdateInventorySlotDisplay.Clear();
		UpdateDisplay();
	}
}

void ULabInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void ULabInventorySlotWidget::HandleUpdateDisplay()
{
	if (InventorySlotEntry && InventorySlotEntry->InventoryItem != nullptr)
	{
		ItemCount = InventorySlotEntry->ItemCount;
		InventoryItem = InventorySlotEntry->InventoryItem;
		UpdateDisplay();
	}
}
