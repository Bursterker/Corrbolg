#include "Actions/CorrbolgRetrieveItem.h"

#include "Inventory/Core/CorrbolgInventoryComponent.h"
#include "Items/Definitions/Fragments/CorrbolgInventoryFragment.h"

#include "Actions/CorrbolgActionContextFragments.h"

void UCorrbolgRetrieveItem::Client_PerformAction_Implementation()
{
	Super::Client_PerformAction_Implementation();

	ensureAlwaysMsgf(false, TEXT("The action RetrieveItem should not be executed on the client!"));

	Finish(ECorrbolgActionResult::Failure);
}

void UCorrbolgRetrieveItem::Server_PerformAction_Implementation()
{
	Super::Server_PerformAction_Implementation();

	UCorrbolgInventoryComponent* const InventoryComponent = Cast<UCorrbolgInventoryComponent>(Context.InventoryComponent);
	if (!ensureMsgf(InventoryComponent, TEXT("Trying to retrieve an item but the InventoryComponent in the context was not valid!")))
	{
		Finish(ECorrbolgActionResult::Failure);
		return;
	}

	const FCorrbolgStorageContextFragment* const StorageFragment = Context.Payload.GetPtr<FCorrbolgStorageContextFragment>();
	if (!ensureMsgf(StorageFragment != nullptr, TEXT("Trying to retrieve an item but the payload was not valid for this action!")))
	{
		Finish(ECorrbolgActionResult::Failure);
		return;
	}

	const UCorrbolgItemDefinition* const ItemDefinition = StorageFragment->Item;
	if (!ensureMsgf(ItemDefinition != nullptr, TEXT("Trying to retrieve an item but the StorageFragment has no item definition!")))
	{
		Finish(ECorrbolgActionResult::Failure);
		return;
	}

	const UCorrbolgInventoryFragment* const InventoryFragment = StorageFragment->Item->FindFragmentOfClass<UCorrbolgInventoryFragment>();
	if (!ensureMsgf(InventoryFragment != nullptr, TEXT("Trying to retrieve an item but the item has no Inventory Fragment!")))
	{
		Finish(ECorrbolgActionResult::Failure);
		return;
	}

	// Look for the item in the inventory, decrease the stack size until the whole amount is retrieved.
	int RemainingStackSize = StorageFragment->StackSize;

	for (FCorrbolgInventoryEntry& Entry : InventoryComponent->GetStoredEntries())
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

	Finish(ECorrbolgActionResult::Success);
}
