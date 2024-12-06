﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LabInventoryGridWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "LabInventorySlotWidget.generated.h"

class ULabItemDraggedPreviewWidget;
class ULabInventoryItem;
class ULabInventorySlotEntry;

UCLASS()
class LABINVENTORY_API ULabDragDropOps : public UDragDropOperation
{
	GENERATED_BODY()

public:
	TWeakObjectPtr<ULabInventorySlotEntry> DraggedSlotItemData;
	
};

UCLASS()
class LABINVENTORY_API ULabInventorySlotWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	bool HasValidItem() const;
	
	int32 GetSlotIndex() const { return SlotItemData->SlotIndex; }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ULabItemDraggedPreviewWidget> ItemDragPreviewWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ULabInventorySlotEntry> SlotItemData;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const ULabInventoryItem> InventoryItem = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemCount = 0;
	
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateDisplay();
	
	// ~Start IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;
	// ~End IUserObjectListEntry

	// ~Start UUserWidget
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	// ~End UUserWidget

private:
	mutable TWeakObjectPtr<UUserWidget> CachedPreviewWidget;
	
	UUserWidget* CreateDragPreviewWidget() const;
	
	void HandleUpdateDisplay();
};
