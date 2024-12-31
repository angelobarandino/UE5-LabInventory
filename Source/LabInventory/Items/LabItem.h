#pragma once

#include "CoreMinimal.h"
#include "LabItemFragment.h"
#include "LabItem.generated.h"

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Item"))
class ULabItem : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	ULabItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup|Display")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup|Display")
	TSoftObjectPtr<UStaticMesh> DisplayMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Fragments")
	TArray<TObjectPtr<ULabItemFragment>> Fragments;
};
