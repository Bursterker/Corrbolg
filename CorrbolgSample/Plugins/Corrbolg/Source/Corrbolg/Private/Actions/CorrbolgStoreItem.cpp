#include "Actions/CorrbolgStoreItem.h"

void UCorrbolgStoreItem::Execute_Server_Implementation(const FActionContext& ActionContext)
{
	Super::Execute_Server_Implementation(ActionContext);

	StoreItem_Server_Implementation();
}

void UCorrbolgStoreItem::StoreItem_Server_Implementation() const
{
	Context.StoredItems->Add(Context.Item);
}