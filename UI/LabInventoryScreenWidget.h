// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LabInventoryScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABINVENTORY_API ULabInventoryScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ULabInventoryScreenWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
