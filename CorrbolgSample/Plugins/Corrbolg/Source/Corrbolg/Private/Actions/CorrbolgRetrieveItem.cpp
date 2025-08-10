#include "Actions/CorrbolgRetrieveItem.h"

#include "Actions/CorrbolgActionContextFragments.h"

void UCorrbolgRetrieveItem::Execute_Server_Implementation(const FCorrbolgActionContext& ActionContext)
{
	Super::Execute_Server_Implementation(ActionContext);

	RetrieveItem_Server_Implementation();
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
