// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LabInventory/Items/LabInventoryList.h"
#include "LabInventory/Items/LabUpdateInventoryParam.h"
#include "LabInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemUpdatedDelegate, const FLabInventoryItemInstance&, Instance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemRemovedDelegate, const int32, RemovedSlotIndex);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
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
	FLabUpdateInventoryParam FindInventorySlotForItem(const int32 ItemCount, const TSoftObjectPtr<ULabItem>& InventoryItem);

	UFUNCTION()
	FLabUpdateInventoryParam CreateMoveToSlotForItem(const int32 SlotIndex, const int32 ItemCount, const TSoftObjectPtr<ULabItem>& InventoryItem);

	UFUNCTION()
	bool CanMoveItemToSlot(const int32 SlotIndex, const ULabItem* InventoryItem);
	
	UFUNCTION()
	bool AddInventoryItem(const FLabUpdateInventoryParam& Params);

	UFUNCTION()
	bool RemoveInventoryItem(const int32 SlotIndex, const int32 ItemCount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FLabInventoryItemInstance> GetInventoryItems() const;
	
private:

	UPROPERTY(Replicated)
	FLabInventoryList InventoryList;

	UFUNCTION()
	bool IsItemCompatible(const FLabInventoryEntry& ItemEntry, const ULabItem* InventoryItem) const;

	UFUNCTION()
	void RetrieveItemStackingInfo(const ULabItem* InventoryItem, bool& bOutStackable, int32& OutStackSize) const;
	
	UFUNCTION()
	int32 CalculateMaxItemsToAdd(int32 MaxStackPerSlot, int32 CurrentItemCountInSlot, int32 RequestedItemCount) const;
};

