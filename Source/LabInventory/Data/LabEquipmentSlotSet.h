// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LabEquipmentSlotSet.generated.h"

USTRUCT()
struct FLabEquipmentSlotSet_SlotInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName SlotName;

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> SlotTypes;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> PlaceholderIcon;
};


UCLASS(meta = (DisplayName = "Equipment Slot Set"))
class LABINVENTORY_API ULabEquipmentSlotSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FLabEquipmentSlotSet_SlotInfo> EquipmentSlots;

};
