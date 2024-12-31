#pragma once

#include "CoreMinimal.h"
#include "LabInventoryItemInstance.generated.h"

struct FLabInventoryList;
class ULabInventoryComponent;
class ULabItem;

USTRUCT(BlueprintType)
struct FLabInventoryItemInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<ULabItem> Item = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemCount = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex = INDEX_NONE;
};
