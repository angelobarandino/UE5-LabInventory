// Fill out your copyright notice in the Description page of Project Settings.


#include "LabInventorySlotWidget.h"

#include "LabInventoryGridWidget.h"
#include "LabItemDraggedPreviewWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "LabInventory/LabInventory.h"
#include "LabInventory/Core/LabUpdateInventoryParam.h"
#include "LabInventory/Interfaces/LabPlayerInventoryInterface.h"


bool ULabInventorySlotWidget::HasValidItem() const
{
	return InventoryItem != nullptr && ItemCount > 0;
}

void ULabInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void ULabInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (ListItemObject)
	{
		SlotItemData = Cast<ULabInventorySlotEntry>(ListItemObject);

		ItemCount = SlotItemData->ItemCount;
		InventoryItem = SlotItemData->InventoryItem;
		SlotItemData->UpdateInventorySlotDisplay.AddUObject(this, &ThisClass::HandleUpdateDisplay);
		
		HandleUpdateDisplay();
	}
}

void ULabInventorySlotWidget::NativeOnEntryReleased()
{
	if (SlotItemData.IsValid())
	{
		ItemCount = 0;
		InventoryItem = nullptr;
		SlotItemData->UpdateInventorySlotDisplay.Clear();
		UpdateDisplay();
	}
}

void ULabInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void ULabInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

FReply ULabInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (HasValidItem())
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}

	return FReply::Unhandled();
}

FReply ULabInventorySlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	if (HasValidItem() && InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	
	return FReply::Unhandled();
}

void ULabInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (HasValidItem() && SlotItemData.IsValid() && SlotItemData->Inventory.IsValid())
	{
		UUserWidget* PreviewWidget = CreateDragPreviewWidget();
		if (!PreviewWidget)
		{
			UE_LOG(LogInventory, Warning, TEXT("Failed to create drag preview widget."));
			return;
		}
		
		if (ULabItemDraggedPreviewWidget* ItemPreviewWidget = Cast<ULabItemDraggedPreviewWidget>(PreviewWidget))
		{
			ItemPreviewWidget->InitializePreview(SlotItemData.Get());
		}
		
		ULabDragDropOps* Operation = Cast<ULabDragDropOps>(UWidgetBlueprintLibrary::CreateDragDropOperation(ULabDragDropOps::StaticClass()));
		if (Operation)
		{
			Operation->DefaultDragVisual = PreviewWidget;
			Operation->DraggedSlotItemData = SlotItemData;
			OutOperation = Operation;				
		}
	}
}

bool ULabInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const ULabDragDropOps* Operation = Cast<ULabDragDropOps>(InOperation);
	if (Operation && Operation->DraggedSlotItemData.IsValid())
	{
		if (APlayerController* PlayerController = GetOwningPlayer())
		{
			if (PlayerController->Implements<ULabPlayerInventoryInterface>())
			{
				FLabMoveInventoryItemParam MoveItemParam;
				MoveItemParam.SourceInventory = Operation->DraggedSlotItemData->Inventory;
				MoveItemParam.SourceSlotIndex = Operation->DraggedSlotItemData->SlotIndex;
				MoveItemParam.TargetInventory = SlotItemData->Inventory;
				MoveItemParam.TargetSlotIndex = SlotItemData->SlotIndex;
				ILabPlayerInventoryInterface::Execute_MoveInventoryItem(PlayerController, MoveItemParam);
			}
			else
			{
				UE_LOG(LogInventory, Warning, TEXT("PlayerController does not implement ULabPlayerInventoryInterface"));
			}
		}
	}

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

UUserWidget* ULabInventorySlotWidget::CreateDragPreviewWidget() const
{
	if (!ItemDragPreviewWidgetClass)
	{
		UE_LOG(LogInventory, Warning, TEXT("ItemDragPreviewWidgetClass is null."));
		return nullptr;
	}
	
	if (CachedPreviewWidget.IsValid())
	{
		return CachedPreviewWidget.Get();
	}

	UUserWidget* PreviewWidget = UWidgetBlueprintLibrary::Create(GetWorld(), ItemDragPreviewWidgetClass, GetOwningPlayer());
	if (PreviewWidget)
	{
		CachedPreviewWidget = PreviewWidget;
	}

	return PreviewWidget;
}

void ULabInventorySlotWidget::HandleUpdateDisplay()
{
	if (SlotItemData.IsValid())
	{
		ItemCount = SlotItemData->ItemCount;
		InventoryItem = SlotItemData->InventoryItem;
		UpdateDisplay();
	}
}
