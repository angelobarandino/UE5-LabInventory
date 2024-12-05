// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LabInventoryGridWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "LabInventorySlotWidget.generated.h"

class ULabInventoryItem;
class ULabInventorySlotEntry;

UCLASS()
class LABINVENTORY_API ULabInventorySlotWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULabInventorySlotEntry> InventorySlotEntry;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const ULabInventoryItem> InventoryItem = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemCount = 0;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateDisplay();

	// ~Start IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;
	// ~End IUserObjectListEntry

	virtual void NativeOnInitialized() override;
	
public:
	
	int32 GetSlotIndex() const { return InventorySlotEntry->SlotIndex; }
	
	void HandleUpdateDisplay();

};
