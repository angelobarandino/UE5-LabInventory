// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LabInventory/Items/LabItem.h"
#include "LabEquipmentItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Equipment Item"))
class LABINVENTORY_API ULabEquipmentItem : public ULabItem
{
	GENERATED_BODY()
};
