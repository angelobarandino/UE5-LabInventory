// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LabPlayerInventoryInterface.generated.h"

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

	virtual ULabInventoryComponent* GetInventoryComponent() const = 0;
	
};
