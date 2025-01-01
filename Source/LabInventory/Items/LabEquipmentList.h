#pragma once

#include "CoreMinimal.h"
#include "LabEquipmentItemInstance.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "LabEquipmentList.generated.h"


class ULabEquipmentItem;
class ULabEquipmentComponent;

USTRUCT(BlueprintType)
struct FLabEquipmentSlot : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FLabEquipmentSlot() : SlotID(NAME_None), Item(nullptr) {}
	FLabEquipmentSlot(const FName InSlotID) : SlotID(InSlotID) {}

	UPROPERTY()
	FName SlotID;

	UPROPERTY()
	TSoftObjectPtr<ULabEquipmentItem> Item;
	
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

	bool AddSlot(const FName SlotID);

	void SetSlotEquipmentItem(const FName SlotID, const TSoftObjectPtr<ULabEquipmentItem>& EquipmentItem);

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