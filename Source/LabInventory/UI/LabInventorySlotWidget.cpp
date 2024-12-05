// Fill out your copyright notice in the Description page of Project Settings.


#include "LabInventorySlotWidget.h"

#include "LabInventoryGridWidget.h"
#include "LabInventory/Items/LabInventoryItem.h"

void ULabInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (ListItemObject)
	{
		InventorySlotEntry = Cast<ULAB_InventorySlotEntry>(ListItemObject);
		InventorySlotEntry->UpdateInventorySlotDisplay.AddUObject(this, &ThisClass::HandleUpdateDisplay);
		HandleUpdateDisplay();
	}
}

void ULabInventorySlotWidget::NativeOnEntryReleased()
{
	if (InventorySlotEntry)
	{
		InventorySlotEntry->ItemCount = 0;
		InventorySlotEntry->InventoryItem = nullptr;
		InventorySlotEntry->UpdateInventorySlotDisplay.Clear();
		UpdateDisplay(0, nullptr);
	}
}

void ULabInventorySlotWidget::HandleUpdateDisplay()
{
	if (InventorySlotEntry && InventorySlotEntry->InventoryItem != nullptr)
	{
		UpdateDisplay(InventorySlotEntry->ItemCount, InventorySlotEntry->InventoryItem);
	}
}
