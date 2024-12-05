// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LabItemFragment.generated.h"

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class LABINVENTORY_API ULabItemFragment : public UObject
{
	GENERATED_BODY()
};

UCLASS(BlueprintType, meta=(DisplayName="Inventory"))
class LABINVENTORY_API ULAB_InventoryFragment : public ULabItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
	bool bStackable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
	int32 StackSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
	TSoftObjectPtr<UTexture2D> Icon;
};