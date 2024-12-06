// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LabInventoryGridWidget.generated.h"

struct FLabInventoryItemInstance;
class UTileView;
class ULabInventoryItem;
class ULabInventoryComponent;

DECLARE_MULTICAST_DELEGATE(FLoadInventorySlotItem);

UCLASS(BlueprintType)
class ULabInventorySlotEntry : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex = INDEX_NONE;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const ULabInventoryItem> InventoryItem = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemCount = 0;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ULabInventoryComponent> Inventory;
	
	FLoadInventorySlotItem UpdateInventorySlotDisplay;
};

UCLASS(Blueprintable)
class LABINVENTORY_API ULabInventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ULabInventoryGridWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void InitInventory(AActor* Owner);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTileView> InventorySlots;

	UFUNCTION(BlueprintImplementableEvent)
	void OnInitializeInventory();
	
private:

	void CreateInventorySlots() const;
		
	UFUNCTION()
	void HandleInventoryItemUpdated(const FLabInventoryItemInstance& Instance);

	UFUNCTION()
	void HandleInventoryItemRemoved(const int RemovedSlotIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> OwnerActor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULabInventoryComponent> OwnerInventory = nullptr;

};
