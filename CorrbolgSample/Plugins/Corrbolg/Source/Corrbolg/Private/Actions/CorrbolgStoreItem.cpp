#include "Actions/CorrbolgStoreItem.h"

#include "Items/Definitions/Fragments/CorrbolgInventoryFragment.h"

#include "Actions/CorrbolgActionContextFragments.h"

void UCorrbolgStoreItem::PerformAction(const FCorrbolgActionContext& ActionContext) const
{
	StoreItem_Server_Implementation();

	OnActionFinished.Broadcast(ECorrbolgActionResult::Success);
}

void UCorrbolgStoreItem::StoreItem_Server_Implementation() const
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

	const FGuid ItemId = ItemDefinition->GetId();
	const FPrimaryAssetId AssetId = ItemDefinition->GetPrimaryAssetId();

	FCorrbolgInventoryEntry EntryData = FCorrbolgInventoryEntry(ItemId, AssetId, StorageFragment->StackSize);

	// TODO: Koen: Use the inventory fragment to check if the item is stackable and then search an existing entry and increase the stacksize instead of adding an entry.
	Context.Inventory->Add(EntryData);
}
