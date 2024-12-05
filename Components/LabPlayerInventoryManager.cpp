// Fill out your copyright notice in the Description page of Project Settings.


#include "LabPlayerInventoryManager.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "LabInventory/UI/LabInventoryScreenWidget.h"


ULabPlayerInventoryManager::ULabPlayerInventoryManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULabPlayerInventoryManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetOwner());
	check(PlayerController);

	if (PlayerController->IsLocalPlayerController())
	{
		if (InventoryScreenClass)
		{
			InventoryScreenWidget = CreateWidget<ULabInventoryScreenWidget>(PlayerController, InventoryScreenClass);
			InventoryScreenWidget->AddToViewport();
			InventoryScreenWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ULabPlayerInventoryManager::ToggleInventory()
{
	if (!IsValid(PlayerController))
		return;

	if (!PlayerController->IsLocalPlayerController())
		return;
	
	if (InventoryScreenClass && !InventoryScreenWidget)
	{
		InventoryScreenWidget = CreateWidget<ULabInventoryScreenWidget>(PlayerController, InventoryScreenClass);
		InventoryScreenWidget->AddToViewport();
		InventoryScreenWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (IsValid(InventoryScreenWidget))
	{
		if (InventoryScreenWidget->IsVisible())
		{
			InventoryScreenWidget->SetVisibility(ESlateVisibility::Hidden);
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
			PlayerController->bShowMouseCursor = false;
		}
		else
		{
			InventoryScreenWidget->SetVisibility(ESlateVisibility::Visible);
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, InventoryScreenWidget);
			PlayerController->bShowMouseCursor = true;
		}
	}
}
