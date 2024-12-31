// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LabInventoryStatics.generated.h"

struct FLabMoveInventoryItemParam;
class ULabInventoryComponent;
class ULabItemFragment;
class ULabItem;

UENUM(BlueprintType)
enum class EInventoryUpdateResult : uint8
{
	
	Failure,			// Represents a failed attempt to modify the inventory
	PartiallyAdded,		// When the operation was partially successful (e.g., some items added)
	Success,			// When the action was fully successful
	FullInventory,		// When the inventory is full, and no more items can be added
	InventoryNotFound	// When the target actor has no attached InventoryComponent
};

USTRUCT(BlueprintType)
struct FLabInventoryTransactionResult
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EInventoryUpdateResult ChangeResult = EInventoryUpdateResult::Failure;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemsAffectedCount = 0;
};

UCLASS()
class LABINVENTORY_API ULabInventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "LAB Inventory Statics")
	static FLabInventoryTransactionResult TryAddItemToInventory(AActor* TargetActor, const int32 ItemCount, const TSoftObjectPtr<ULabItem>& ItemDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "LAB Inventory Statics")
	static void MoveInventoryItem(const FLabMoveInventoryItemParam& MoveItemParam);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "LAB Inventory Statics")
	static void TransferAllInventoryItems(ULabInventoryComponent* SourceInventory, ULabInventoryComponent* TargetInventory);
	
	UFUNCTION(BlueprintCallable, Category = "LAB Inventory Statics", meta=(DeterminesOutputType=FragmentClass))
	static const ULabItemFragment* FindItemDefinitionFragment(const ULabItem* InventoryItem, TSubclassOf<ULabItemFragment> FragmentClass);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LAB Inventory Statics")
	static ULabInventoryComponent* GetInventoryComponent(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LAB Inventory Statics")
	static const ULabInventoryComponent* GetInventoryComponentConst(AActor* Actor);

	template<typename TFragmentClass>
	static const TFragmentClass* FindItemDefinitionFragment(const ULabItem* InventoryItem)
	{
		return Cast<TFragmentClass>(FindItemDefinitionFragment(InventoryItem, TFragmentClass::StaticClass()));
	}
};
