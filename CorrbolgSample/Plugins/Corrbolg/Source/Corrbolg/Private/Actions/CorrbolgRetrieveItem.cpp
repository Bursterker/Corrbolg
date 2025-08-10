#include "Actions/CorrbolgRetrieveItem.h"

#include "Actions/CorrbolgActionContextFragments.h"

ECorrbolgActionResult UCorrbolgRetrieveItem::PerformAction_Server(const FCorrbolgActionContext& ActionContext) const
{
	RetrieveItem_Server_Implementation();

	return ECorrbolgActionResult::Success;
}

void UCorrbolgRetrieveItem::RetrieveItem_Server_Implementation() const
{
	const FCorrbolgStorageContextFragment* const StorageFragment = Context.Payload.GetPtr<FCorrbolgStorageContextFragment>();

	if (!ensureMsgf(StorageFragment != nullptr, TEXT("Trying to retrieve an item but the payload was not valid for this action!")))
	{
		return;
	}

	Context.StoredItems->RemoveSingle(StorageFragment->Item);
}
