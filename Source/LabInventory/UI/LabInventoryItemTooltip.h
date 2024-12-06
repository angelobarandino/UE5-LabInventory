// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LabInventoryItemTooltip.generated.h"

class ULabInventoryItem;
/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class LABINVENTORY_API ULabInventoryItemTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitTooltip(const int32 ItemCount, const ULabInventoryItem* InventoryItem);
};
