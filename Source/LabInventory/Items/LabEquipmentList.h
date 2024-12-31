#pragma once

#include "CoreMinimal.h"
#include "LabEquipmentItemInstance.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "LabEquipmentList.generated.h"


class ULabEquipmentComponent;

USTRUCT(BlueprintType)
struct FLabEquipmentSlot : public FFastArraySerializerItem
{
	GENERATED_BODY()

private:
	
	UPROPERTY()
	FName SlotID = NAME_None;

	UPROPERTY()
	FLabEquipmentItemInstance Instance;
	
};

USTRUCT(BlueprintType)
struct FLabEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FLabEquipmentList() : OwningEquipmentComponent(nullptr) {}
	FLabEquipmentList(ULabEquipmentComponent* InEquipmentComponent)
		: OwningEquipmentComponent(InEquipmentComponent) {}

	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FLabEquipmentSlot, FLabEquipmentList>( Items, DeltaParms, *this );
	}
	
private:
	friend ULabEquipmentComponent;
	
	UPROPERTY()
	TArray<FLabEquipmentSlot> Items;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<ULabEquipmentComponent> OwningEquipmentComponent;
};

template<>
struct TStructOpsTypeTraits<FLabEquipmentList> : public TStructOpsTypeTraitsBase2<FLabEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};