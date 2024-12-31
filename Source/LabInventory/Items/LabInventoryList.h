#pragma once

#include "CoreMinimal.h"
#include "LabInventoryItemInstance.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "LabInventoryList.generated.h"

struct FLabUpdateInventoryParam;
class ULabInventoryComponent;
class ULabItem;

USTRUCT(BlueprintType)
struct FLabInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
private:
	friend class ULabInventoryComponent;
	friend struct FLabInventoryList;
	
	UPROPERTY()
	FLabInventoryItemInstance Instance;

	UPROPERTY(NotReplicated)
	TObjectPtr<ULabInventoryComponent> InventoryComponent;
};

USTRUCT(BlueprintType)
struct FLabInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FLabInventoryList() : OwnerInventory(nullptr) {}
	FLabInventoryList(ULabInventoryComponent* InInventoryComponent)
		: OwnerInventory(InInventoryComponent) {}
	
	// ~Start FFastArraySerializer
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FLabInventoryEntry, FLabInventoryList>( Items, DeltaParms, *this );
	}
	// ~End FFastArraySerializer

	FLabInventoryEntry* GetItemAtSlot(const int32 SlotIndex);

private:
	friend class ULabInventoryComponent;
	
	bool AddItem(const int32 SlotIndex, const int32 ItemCount, const TSoftObjectPtr<ULabItem>& InventoryItem);
	
	bool AddItemCount(const int32 SlotIndex, const int32 ItemCount);

	bool RemoveItemCount(const int32 SlotIndex, const int32 ItemCount);
	
	UPROPERTY()
	TArray<FLabInventoryEntry> Items;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<ULabInventoryComponent> OwnerInventory;
};

template<>
struct TStructOpsTypeTraits<FLabInventoryList> : public TStructOpsTypeTraitsBase2<FLabInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};