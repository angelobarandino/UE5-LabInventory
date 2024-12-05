#pragma once

#include "CoreMinimal.h"
#include "LabAddItemParams.generated.h"


class ULabInventoryItem;

UENUM()
enum EItemSlotStatus
{
	InventoryFull,
	InsertToSlot,
	UpdateItemSlot,
	UnavailableItemSlot,
	InventoryItemInvalid,
};

USTRUCT()
struct FLabAddItemParams
{
	GENERATED_BODY()

	UPROPERTY()
	int32 SlotIndex = -1;
	
	UPROPERTY()
	int32 StackSize = 1;

	UPROPERTY()
	bool bStackable = true;

	UPROPERTY()
	int32 SlotCurrentItemCount = 0;

	// TODO: Shorten the name of this field
	UPROPERTY()
	int32 SlotMaxAvailableItems = 0;

	UPROPERTY()
	TSoftObjectPtr<ULabInventoryItem> InventoryItem = nullptr;

	UPROPERTY()
	TEnumAsByte<EItemSlotStatus> Status = InventoryFull;
};
