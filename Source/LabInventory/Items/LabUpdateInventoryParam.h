﻿#pragma once

#include "CoreMinimal.h"
#include "LabUpdateInventoryParam.generated.h"


class ULabInventoryComponent;
class ULabItem;

UENUM()
enum ELabInventorySlotStatus
{
	InventoryFull,
	InsertToSlot,
	UpdateItemSlot,
	UnavailableItemSlot,
	InventoryItemInvalid,
};

USTRUCT(BlueprintType)
struct FLabUpdateInventoryParam
{
	GENERATED_BODY()

	int32 SlotIndex = -1;
	
	UPROPERTY()
	int32 StackSize = 1;

	UPROPERTY()
	bool bStackable = true;

	UPROPERTY()
	int32 SlotCurrentItemCount = 0;

	UPROPERTY()
	int32 RemainingSlotCapacity = 0;

	UPROPERTY()
	TSoftObjectPtr<ULabItem> InventoryItem = nullptr;

	UPROPERTY()
	TEnumAsByte<ELabInventorySlotStatus> Status = InventoryFull;

	FORCEINLINE bool CanAddItems() const
	{
		return Status == InsertToSlot || Status == UpdateItemSlot;
	}
};

USTRUCT(BlueprintType)
struct FLabMoveInventoryItemParam
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ULabInventoryComponent> SourceInventory;

	UPROPERTY(BlueprintReadOnly)
	int32 SourceSlotIndex;
	
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ULabInventoryComponent> TargetInventory;
	
	UPROPERTY(BlueprintReadOnly)
	int32 TargetSlotIndex;
};