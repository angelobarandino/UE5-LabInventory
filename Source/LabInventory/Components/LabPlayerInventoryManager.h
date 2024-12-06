// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LabInventory/Interfaces/LabInventoryManagerInterface.h"
#include "LabPlayerInventoryManager.generated.h"


class ULabInventoryItemTooltip;
class ULabItemDraggedPreviewWidget;
class ULabInventoryScreenWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LABINVENTORY_API ULabPlayerInventoryManager : public UActorComponent, public ILabInventoryManagerInterface
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ULabInventoryScreenWidget> InventoryScreenClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ULabInventoryItemTooltip> InventoryTooltipClass;
	
public:
	// Sets default values for this component's properties
	ULabPlayerInventoryManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	virtual UUserWidget* GetOrCreateItemPreview(const UObject* Outer, const TSubclassOf<UUserWidget>& PreviewWidgetClass) override;
	
	virtual TSubclassOf<ULabInventoryItemTooltip> GetInventoryTooltipClass() const override { return InventoryTooltipClass; }

private:

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY()
	TObjectPtr<ULabInventoryScreenWidget> InventoryScreenWidget;
	
	UPROPERTY()
	TWeakObjectPtr<UUserWidget> CachedPreviewWidget;
};
