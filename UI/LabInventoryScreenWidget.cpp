// Fill out your copyright notice in the Description page of Project Settings.


#include "LabInventoryScreenWidget.h"

#include "LabInventory/Components/LabPlayerInventoryManager.h"

ULabInventoryScreenWidget::ULabInventoryScreenWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

FReply ULabInventoryScreenWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		if (const APlayerController* PC = GetOwningPlayer())
		{
			if (ULabPlayerInventoryManager* InventoryManager = PC->FindComponentByClass<ULabPlayerInventoryManager>())
			{
				InventoryManager->ToggleInventory();
			}
		}
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
