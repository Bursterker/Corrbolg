#include "Actions/CorrbolgRetrieveItem.h"

#include "Actions/CorrbolgActionContextFragments.h"

void UCorrbolgRetrieveItem::PerformAction(const FCorrbolgActionContext& ActionContext) const
{
	RetrieveItem_Server_Implementation();

	OnActionFinished.Broadcast(ECorrbolgActionResult::Success);
}

void UCorrbolgRetrieveItem::RetrieveItem_Server_Implementation() const
{
	const FCorrbolgStorageContextFragment* const StorageFragment = Context.Payload.GetPtr<FCorrbolgStorageContextFragment>();
	if (!ensureMsgf(StorageFragment != nullptr, TEXT("Trying to retrieve an item but the payload was not valid for this action!")))
	{
		return;
	}

	// TODO: Koen: No need for the whole item definition, only pass the ObjectId in the payload.
	const UCorrbolgItemDefinition* const ItemDefinition = StorageFragment->Item;
	if (!ensureMsgf(ItemDefinition != nullptr, TEXT("Trying to retrieve an item but the StorageFragment has no item definition!")))
	{
		return;
	}

	const int EntryIndex = Context.Inventory->IndexOfByPredicate([&ItemDefinition](const FCorrbolgInventoryEntry& InventoryEntry)
	{
		return ItemDefinition->GetId() == InventoryEntry.GetObjectId();
	});

	if (EntryIndex == INDEX_NONE)
	{
		return;
	}

	Context.Inventory->RemoveAt(EntryIndex);
}
