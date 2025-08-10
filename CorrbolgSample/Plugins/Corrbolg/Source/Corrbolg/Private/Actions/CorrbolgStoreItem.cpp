#include "Actions/CorrbolgStoreItem.h"

#include "Actions/CorrbolgActionContextFragments.h"

ECorrbolgActionResult UCorrbolgStoreItem::PerformAction_Server(const FCorrbolgActionContext& ActionContext) const
{
	StoreItem_Server_Implementation();

	return ECorrbolgActionResult::Success;
}

void UCorrbolgStoreItem::StoreItem_Server_Implementation() const
{
	const FCorrbolgStorageContextFragment* const StorageFragment = Context.Payload.GetPtr<FCorrbolgStorageContextFragment>();

	if (!ensureMsgf(StorageFragment != nullptr, TEXT("Trying to store an item but the payload was not valid for this action!")))
	{
		return;
	}

	Context.StoredItems->Add(StorageFragment->Item);
}
