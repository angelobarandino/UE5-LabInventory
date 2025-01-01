#include "LabEquipmentList.h"

bool FLabEquipmentList::AddSlot(const FName SlotID)
{
	if (SlotID == NAME_None)
	{
		return false;
	}
	
	const bool SlotExist = Items.ContainsByPredicate([SlotID](const FLabEquipmentSlot& EquipmentSlot)
	{
		return EquipmentSlot.SlotID == SlotID;
	});

	if (SlotExist)
	{
		return false;
	}

	Items.Add(FLabEquipmentSlot(SlotID));
	MarkArrayDirty();
	return true;
}

void FLabEquipmentList::SetSlotEquipmentItem(const FName SlotID, const TSoftObjectPtr<ULabEquipmentItem>& EquipmentItem)
{
	for (FLabEquipmentSlot& EquipmentSlot : Items)
	{
		if (EquipmentSlot.SlotID == SlotID)
		{
			EquipmentSlot.Item = EquipmentItem;
			MarkItemDirty(EquipmentSlot);
		}
	}
}
