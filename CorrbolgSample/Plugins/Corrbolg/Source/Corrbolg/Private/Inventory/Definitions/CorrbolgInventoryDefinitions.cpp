// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Definitions/CorrbolgInventoryDefinitions.h"

FCorrbolgInventoryEntry::FCorrbolgInventoryEntry(const FGuid& Id, const FPrimaryAssetId& AssetId, const int Count)
{
	ObjectId = Id;
	PrimaryAssetId = AssetId;
	StackSize = Count;
}

#pragma region Core
const bool FCorrbolgInventoryEntry::IsValid() const
{
	const bool bHasValidId = ObjectId.IsValid();
	const bool bHasValidAssetId = PrimaryAssetId.IsValid();
	const bool bHasValidStackSize = StackSize > 0;

	return bHasValidId && bHasValidAssetId && bHasValidStackSize;
}

void FCorrbolgInventoryEntry::Reset()
{
	ObjectId.Invalidate();
	PrimaryAssetId = FPrimaryAssetId();
	StackSize = 0;
}
#pragma endregion

#pragma region Modifiers
void FCorrbolgInventoryEntry::IncreaseStackSize(const int32 Amount)
{
	StackSize += Amount;
}

int FCorrbolgInventoryEntry::DecreaseStackSize(const int32 Amount)
{
	StackSize -= Amount;

	// If the stack size is 0 or less, reset the entry and return the remaining amount that was not removed from the stack.
	if (StackSize <= 0)
	{
		const int RemainingAmount = -StackSize;

		Reset();

		return RemainingAmount;
	}

	// The stack size was decreased but is still valid, so there is no remaining amount.
	return 0;
}
#pragma endregion
