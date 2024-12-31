// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LabEquipmentComponent.generated.h"


struct FLabEquipmentList;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LABINVENTORY_API ULabEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULabEquipmentComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	virtual void BeginPlay() override;

public:
		

private:
	UPROPERTY(Replicated)
	TArray<FLabEquipmentList> Equipments;
};
