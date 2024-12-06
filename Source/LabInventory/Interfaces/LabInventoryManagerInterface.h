// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LabInventoryManagerInterface.generated.h"

class ULabInventoryItemTooltip;

// This class does not need to be modified.
UINTERFACE()
class ULabInventoryManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LABINVENTORY_API ILabInventoryManagerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual UUserWidget* GetOrCreateItemPreview(const UObject* Outer, const TSubclassOf<UUserWidget>& PreviewWidgetClass) = 0;
	
	virtual TSubclassOf<ULabInventoryItemTooltip> GetInventoryTooltipClass() const = 0;
};
