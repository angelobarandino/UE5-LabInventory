#pragma once

#include "CoreMinimal.h"
#include "LabItemFragment.h"
#include "LabInventoryItem.generated.h"

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Inventory Item"))
class ULabInventoryItem : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	ULabInventoryItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup|Display")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup|Display")
	TSoftObjectPtr<UStaticMesh> DisplayMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Fragments")
	TArray<TObjectPtr<ULabItemFragment>> Fragments;
};
