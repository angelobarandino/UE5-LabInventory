// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LabInventory/Core/LabAddItemParams.h"
#include "LabInventory/Core/LabInventoryData.h"
#include "LabInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemUpdatedDelegate, const FLabInventoryItemInstance&, Instance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemRemovedDelegate, const FLabInventoryItemInstance&, Instance);

UCLASS(meta=(BlueprintSpawnableComponent))
class LABINVENTORY_API ULabInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULabInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FInventoryItemUpdatedDelegate OnInventoryItemUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FInventoryItemRemovedDelegate OnInventoryItemRemoved;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Inventory")
	int32 ItemsSize = 50;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryGetInventoryItemAtSlot(const int32 SlotIndex, FLabInventoryItemInstance& InventoryItem);

	UFUNCTION()
	FLabAddItemParams FindInventorySlotForItem(const int32 ItemCount, const TSoftObjectPtr<ULabInventoryItem>& InventoryItem);
	
	UFUNCTION()
	bool AddInventoryItem(const FLabAddItemParams& Params);
	
private:

	UPROPERTY(Replicated)
	FLabInventoryList InventoryList;

	UFUNCTION()
	void RetrieveItemStackingInfo(const ULabInventoryItem* InventoryItem, bool& bOutStackable, int32& OutStackSize) const;
	
	UFUNCTION()
	int32 CalculateMaxItemsToAdd(int32 MaxStackPerSlot, int32 CurrentItemCountInSlot, int32 RequestedItemCount) const;
};
