// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "LabInventorySlotWidget.generated.h"

class ILabInventoryManagerInterface;
class ULabInventoryItemTooltip;
class ULabItemDraggedPreviewWidget;
class ULabItem;
class ULabInventorySlotEntry;

UCLASS()
class LABINVENTORY_API ULabDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	TObjectPtr<class ULabInventorySlotWidget> DraggedWidget;
};


UCLASS()
class LABINVENTORY_API ULabInventorySlotWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool HasValidItem() const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetSlotIndex() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ULabItemDraggedPreviewWidget> ItemDragPreviewWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ULabInventorySlotEntry> SlotItemData;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const ULabItem> InventoryItem = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemCount = 0;
	
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateDisplay();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnDragStarted();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnDragEnded(const bool bIsCanceled);

	// ~Start IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;
	// ~End IUserObjectListEntry

	// ~Start UUserWidget
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	// ~End UUserWidget

private:
	ILabInventoryManagerInterface* InventoryManager;
	
	TWeakObjectPtr<ULabInventoryItemTooltip> CachedTooltipWidget;
	
	UUserWidget* CreateDragPreviewWidget();
	
	void HandleUpdateDisplay();
	
	void InitTooltipWidget();
};
