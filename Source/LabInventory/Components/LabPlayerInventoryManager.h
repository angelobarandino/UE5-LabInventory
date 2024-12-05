// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LabPlayerInventoryManager.generated.h"


class ULabInventoryScreenWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LABINVENTORY_API ULabPlayerInventoryManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ULabInventoryScreenWidget> InventoryScreenClass;
	
public:
	// Sets default values for this component's properties
	ULabPlayerInventoryManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

private:

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY()
	TObjectPtr<ULabInventoryScreenWidget> InventoryScreenWidget;
};
