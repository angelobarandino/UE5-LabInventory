﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "LabItemDraggedPreviewWidget.h"
#include "LabInventoryGridWidget.h"
#include "LabInventory/Items/LabInventoryItem.h"

void ULabItemDraggedPreviewWidget::InitializePreview(const ULabInventorySlotEntry* SlotEntry)
{
	if (SlotEntry)
	{
        SetItemDetails(SlotEntry->InventoryItem, SlotEntry->ItemCount);
	}
}