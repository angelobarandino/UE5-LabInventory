// Fill out your copyright notice in the Description page of Project Settings.


#include "LabEquipmentComponent.h"

#include "LabInventory/LabInventory.h"
#include "LabInventory/Data/LabEquipmentSlotSet.h"
#include "Net/UnrealNetwork.h"


ULabEquipmentComponent::ULabEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void ULabEquipmentComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Equipments);
}

void ULabEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (EquipmentSlotSet != nullptr && GetOwner()->HasAuthority())
	{
		EquipmentSlotSet->GiveEquipmentSlots(this);
	}
} 

void ULabEquipmentComponent::AddSlot(const FName SlotId)
{
	if (Equipments.AddSlot(SlotId))
	{
		UE_LOG(LogLabInventory, Log, TEXT("AddSlot: Equipment Slot '%s' has been added."), *SlotId.ToString());
	}
}

void ULabEquipmentComponent::EquipItem(const FName SlotID, const TSoftObjectPtr<ULabEquipmentItem>& EquipmentItem)
{
	if (EquipmentItem.IsValid() || SlotID == NAME_None)
		return;
	
	Equipments.SetSlotEquipmentItem(SlotID, EquipmentItem);
}
 