// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LabInventoryGridWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "LabInventorySlotWidget.generated.h"

class ULabInventoryItem;
class ULAB_InventorySlotEntry;

UCLASS()
class LABINVENTORY_API ULabInventorySlotWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULAB_InventorySlotEntry> InventorySlotEntry;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateDisplay(const int32 ItemCount, const ULabInventoryItem* InventoryItem);

	// ~Start IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;
	// ~End IUserObjectListEntry
	
public:
	
	int32 GetSlotIndex() const { return InventorySlotEntry->SlotIndex; }
	
	void HandleUpdateDisplay();

};
