// Fill out your copyright notice in the Description page of Project Settings.


#include "LabEquipmentComponent.h"

#include "Net/UnrealNetwork.h"


ULabEquipmentComponent::ULabEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULabEquipmentComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Equipments);
}

void ULabEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}
