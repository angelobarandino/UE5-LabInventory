// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LabItemDraggedPreviewWidget.generated.h"

class ULabItem;
class ULabInventorySlotEntry;
/**
 * 
 */
UCLASS()
class LABINVENTORY_API ULabItemDraggedPreviewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializePreview(const ULabInventorySlotEntry* SlotEntry);

protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetItemDetails(const ULabItem* Item, const int32 ItemCount);

};
