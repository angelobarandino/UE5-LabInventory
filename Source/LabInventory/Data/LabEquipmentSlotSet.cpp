// Fill out your copyright notice in the Description page of Project Settings.


#include "LabEquipmentSlotSet.h"

#include "LabInventory/Components/LabEquipmentComponent.h"

void ULabEquipmentSlotSet::GiveEquipmentSlots(ULabEquipmentComponent* EquipmentComponent)
{
	if (EquipmentComponent == nullptr)
		return;
	
	for (auto EquipmentSlot : EquipmentSlots)
	{
		EquipmentComponent->AddSlot(EquipmentSlot.Key);
	}	
}
