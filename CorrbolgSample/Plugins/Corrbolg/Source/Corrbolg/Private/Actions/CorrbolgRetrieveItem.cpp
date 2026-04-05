#include "Actions/CorrbolgRetrieveItem.h"

#include "Items/Definitions/Fragments/CorrbolgInventoryFragment.h"

#include "Actions/CorrbolgActionContextFragments.h"

void UCorrbolgRetrieveItem::PerformAction(const FCorrbolgActionContext& ActionContext)
{
	RetrieveItem_Server_Implementation();

	FinishAction(ECorrbolgActionResult::Success);
}

void UCorrbolgRetrieveItem::RetrieveItem_Server_Implementation()
{
	const FCorrbolgStorageContextFragment* const StorageFragment = Context.Payload.GetPtr<FCorrbolgStorageContextFragment>();
	if (!ensureMsgf(StorageFragment != nullptr, TEXT("Trying to retrieve an item but the payload was not valid for this action!")))
	{
		return;
	}

	const UCorrbolgItemDefinition* const ItemDefinition = StorageFragment->Item;
	if (!ensureMsgf(ItemDefinition != nullptr, TEXT("Trying to retrieve an item but the StorageFragment has no item definition!")))
	{
		return;
	}

	const UCorrbolgInventoryFragment* const InventoryFragment = StorageFragment->Item->FindFragmentOfClass<UCorrbolgInventoryFragment>();
	if (!ensureMsgf(InventoryFragment != nullptr, TEXT("Trying to retrieve an item but the item has no Inventory Fragment!")))
	{
		return;
	}

	// Look for the item in the inventory, decrease the stack size until the whole amount is retrieved.
	int RemainingStackSize = StorageFragment->StackSize;

	for (FCorrbolgInventoryEntry& Entry : *Context.Inventory)
	{
		const bool bIsSameItem = Entry.GetObjectId() == ItemDefinition->GetId();

		if (bIsSameItem)
		{
			RemainingStackSize = Entry.DecreaseStackSize(RemainingStackSize);

			if(RemainingStackSize <= 0)
			{
				break;
			}
		}
	}
}
