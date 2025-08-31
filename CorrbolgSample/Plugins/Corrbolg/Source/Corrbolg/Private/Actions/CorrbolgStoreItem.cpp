#include "Actions/CorrbolgStoreItem.h"

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

	Context.StoredItems->Add(StorageFragment->Item);
}
