﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LabInventoryItemTooltip.generated.h"

class ULabItem;
/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class LABINVENTORY_API ULabInventoryItemTooltip : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeTooltip(const int32 ItemCount, const ULabItem* InventoryItem);
};
