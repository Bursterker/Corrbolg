#include "Actions/CorrbolgStoreItem.h"

#include "Items/Definitions/Fragments/CorrbolgInventoryFragment.h"

#include "Actions/CorrbolgActionContextFragments.h"

void UCorrbolgStoreItem::PerformAction(const FCorrbolgActionContext& ActionContext)
{
	StoreItem_Server_Implementation();
}

void UCorrbolgStoreItem::StoreItem_Server_Implementation()
{
	const FCorrbolgStorageContextFragment* const StorageFragment = Context.Payload.GetPtr<FCorrbolgStorageContextFragment>();
	if (!ensureMsgf(StorageFragment != nullptr, TEXT("Trying to store an item but the payload was not valid for this action!")))
	{
		return;
	}

	const UCorrbolgItemDefinition* const ItemDefinition = StorageFragment->Item;
	if (!ensureMsgf(ItemDefinition != nullptr, TEXT("Trying to store an item but the StorageFragment has no item definition!")))
	{
		return;
	}

	const UCorrbolgInventoryFragment* const InventoryFragment = StorageFragment->Item->FindFragmentOfClass<UCorrbolgInventoryFragment>();
	if (!ensureMsgf(InventoryFragment != nullptr, TEXT("Trying to store an item but the item has no Inventory Fragment!")))
	{
		return;
	}

	// If the item is stackable, check if there is an entry and increase the stack size.
	int RemainingStackSize = StorageFragment->StackSize;

	if (InventoryFragment->GetIsStackable())
	{
		for (FCorrbolgInventoryEntry& Entry : *Context.Inventory)
		{
			const bool bIsSameItem = Entry.GetObjectId() == ItemDefinition->GetId(); // Is this entry the same item?
			const bool bHasStackLimit = InventoryFragment->GetRecommendedMaxStackSize() > 0; // Is there a stack limit?
			const bool bCanIncreaseStack = Entry.GetStackSize() < InventoryFragment->GetRecommendedMaxStackSize(); // Entry stack smaller than the stack limit?

			if (bIsSameItem && (!bHasStackLimit || bCanIncreaseStack))
			{
				const int SpaceLeftInStack = InventoryFragment->GetRecommendedMaxStackSize() - Entry.GetStackSize();
				const int AmountToAddToStack = bHasStackLimit ? FMath::Min(SpaceLeftInStack, RemainingStackSize) : RemainingStackSize;

				Entry.IncreaseStackSize(AmountToAddToStack);
				RemainingStackSize -= AmountToAddToStack;

				if (RemainingStackSize <= 0)
				{
					break;
				}
			}
		}
	}

	// If there is still some of the stack left, add a new entry to the inventory.
	if(RemainingStackSize > 0)
	{
		const FGuid ItemId = ItemDefinition->GetId();
		const FPrimaryAssetId AssetId = ItemDefinition->GetPrimaryAssetId();
		
		FCorrbolgInventoryEntry EntryData = FCorrbolgInventoryEntry(ItemId, AssetId, RemainingStackSize);
		
		// Are there any invalid entries to overwrite?
		FCorrbolgInventoryEntry* const AvailableEntry = Context.Inventory->FindByPredicate([](const FCorrbolgInventoryEntry& Entry)
		{
			return !Entry.IsValid();
		});

		if (AvailableEntry)
		{
			*AvailableEntry = EntryData;
		}
		else
		{
			Context.Inventory->Add(EntryData);
		}
	}
}
