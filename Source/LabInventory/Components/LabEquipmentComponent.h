// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LabInventory/Items/LabEquipmentList.h"
#include "LabEquipmentComponent.generated.h"


class ULabEquipmentItem;
class ULabEquipmentSlotSet;
struct FLabEquipmentList;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LABINVENTORY_API ULabEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULabEquipmentComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<ULabEquipmentSlotSet> EquipmentSlotSet;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddSlot(const FName SlotId);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void EquipItem(const FName SlotID, const TSoftObjectPtr<ULabEquipmentItem>& EquipmentItem);
	
private:
	UPROPERTY(Replicated)
	FLabEquipmentList Equipments;
};
