// Fill out your copyright notice in the Description page of Project Settings.


#include "LabInventorySlotWidget.h"

#include "LabInventoryGridWidget.h"
#include "LabInventoryItemTooltip.h"
#include "LabItemDraggedPreviewWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "LabInventory/LabInventory.h"
#include "LabInventory/Components/LabInventoryComponent.h"
#include "LabInventory/Components/LabPlayerInventoryManager.h"
#include "LabInventory/Core/LabUpdateInventoryParam.h"
#include "LabInventory/Interfaces/LabInventoryManagerInterface.h"
#include "LabInventory/Interfaces/LabPlayerInventoryInterface.h"
#include "LabInventory/Items/LabItemFragment.h"
#include "LabInventory/Library/LabInventoryStatics.h"


bool ULabInventorySlotWidget::HasValidItem() const
{
	return InventoryItem != nullptr && ItemCount > 0;
}

int32 ULabInventorySlotWidget::GetSlotIndex() const
{
	return SlotItemData->SlotIndex;
}

void ULabInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const APlayerController* PlayerController = GetOwningPlayer())
	{
		InventoryManager = PlayerController->FindComponentByClass<ULabPlayerInventoryManager>();
	}
}

void ULabInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (ListItemObject)
	{
		SlotItemData = Cast<ULabInventorySlotEntry>(ListItemObject);

		ItemCount = SlotItemData->ItemCount;
		InventoryItem = SlotItemData->InventoryItem;
		SlotItemData->UpdateInventorySlotDisplay.AddUObject(this, &ThisClass::HandleUpdateDisplay);

		InitTooltipWidget();
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

void ULabInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	OnDragEnded(true);
}

void ULabInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (HasValidItem() && SlotItemData.IsValid() && SlotItemData->Inventory.IsValid())
	{
		UUserWidget* PreviewWidget = CreateDragPreviewWidget();
		if (!PreviewWidget)
		{
			UE_LOG(LogLabInventory, Warning, TEXT("Failed to create drag preview widget."));
			return;
		}
		
		if (ULabItemDraggedPreviewWidget* ItemPreviewWidget = Cast<ULabItemDraggedPreviewWidget>(PreviewWidget))
		{
			ItemPreviewWidget->InitializePreview(SlotItemData.Get());
		}
		
		ULabDragDropOperation* Operation = Cast<ULabDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(ULabDragDropOperation::StaticClass()));
		if (Operation)
		{
			Operation->DefaultDragVisual = PreviewWidget;
			Operation->Payload = SlotItemData.Get();
			Operation->Pivot = EDragPivot::TopLeft;
			Operation->DraggedWidget = this;
			OutOperation = Operation;

			OnDragStarted();
		}
	}
}

bool ULabInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if (const ULabDragDropOperation* Operation = Cast<ULabDragDropOperation>(InOperation))
	{
		if (const ULabInventorySlotEntry* DragSlotData = Cast<ULabInventorySlotEntry>(Operation->Payload))
		{
			if (APlayerController* PlayerController = GetOwningPlayer())
			{
				const bool bCanMove = SlotItemData->Inventory->CanMoveItemToSlot(SlotItemData->SlotIndex, DragSlotData->InventoryItem);
				if (!bCanMove || (DragSlotData->Inventory.Get() == SlotItemData->Inventory.Get() && DragSlotData->SlotIndex == SlotItemData->SlotIndex))
				{
					Operation->DraggedWidget->OnDragEnded(true);
					return true;
				}
				
				if (PlayerController->Implements<ULabPlayerInventoryInterface>())
				{
					FLabMoveInventoryItemParam MoveItemParam;
					MoveItemParam.SourceInventory = DragSlotData->Inventory;
					MoveItemParam.SourceSlotIndex = DragSlotData->SlotIndex;
					MoveItemParam.TargetInventory = SlotItemData->Inventory;
					MoveItemParam.TargetSlotIndex = SlotItemData->SlotIndex;
					ILabPlayerInventoryInterface::Execute_MoveInventoryItem(PlayerController, MoveItemParam);

					if (Operation->DraggedWidget)
					{
						Operation->DraggedWidget->OnDragEnded(false);
					}
		
					return true;
				}
			}

			UE_LOG(LogLabInventory, Warning, TEXT("PlayerController is missing or does not implement ULabPlayerInventoryInterface"));
		}
		else
		{
			UE_LOG(LogLabInventory, Warning, TEXT("Dropped item contains missing or invalid payload."));
		}
	}

	return true;
}

UUserWidget* ULabInventorySlotWidget::CreateDragPreviewWidget()
{
	if (!ItemDragPreviewWidgetClass)
	{
		UE_LOG(LogLabInventory, Warning, TEXT("ItemDragPreviewWidgetClass is null."));
		return nullptr;
	}

	if (InventoryManager)
	{
		return InventoryManager->GetOrCreateItemPreview(this, ItemDragPreviewWidgetClass);
	}
	
	return CreateWidget<UUserWidget>(this, ItemDragPreviewWidgetClass);
}

void ULabInventorySlotWidget::HandleUpdateDisplay()
{
	if (SlotItemData.IsValid())
	{
		ItemCount = SlotItemData->ItemCount;
		InventoryItem = SlotItemData->InventoryItem;

		InitTooltipWidget();
		UpdateDisplay();
	}
}

void ULabInventorySlotWidget::InitTooltipWidget()
{
	if (InventoryItem)
	{
		const ULabInventoryFragment* InventoryFragment = ULabInventoryStatics::FindItemDefinitionFragment<ULabInventoryFragment>(InventoryItem);
		
		if (!CachedTooltipWidget.IsValid())
		{
			if (InventoryFragment->TooltipWidgetClass)
			{
				CachedTooltipWidget = CreateWidget<ULabInventoryItemTooltip>(this, InventoryFragment->TooltipWidgetClass);
			}
			else
			{
				const TSubclassOf<ULabInventoryItemTooltip> ItemTooltipWidgetClass = InventoryManager->GetInventoryTooltipClass();
				if (ItemTooltipWidgetClass)
				{
					CachedTooltipWidget = CreateWidget<ULabInventoryItemTooltip>(this, ItemTooltipWidgetClass);
				}
			}
		}

		if (CachedTooltipWidget.IsValid())
		{
			CachedTooltipWidget->InitializeTooltip(ItemCount, InventoryItem);
			SetToolTip(CachedTooltipWidget.Get());
		}
	}
	else
	{
		SetToolTip(nullptr);
	}
}
