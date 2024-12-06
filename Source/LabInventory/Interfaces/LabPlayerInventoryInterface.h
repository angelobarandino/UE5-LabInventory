// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LabPlayerInventoryInterface.generated.h"

struct FLabMoveInventoryItemParam;
class ULabInventoryComponent;

// This class does not need to be modified.
UINTERFACE()
class ULabPlayerInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LABINVENTORY_API ILabPlayerInventoryInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveInventoryItem(const FLabMoveInventoryItemParam& MoveItemParam);
	
};
