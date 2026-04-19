#include "Actions/CorrbolgStoreItem.h"

#include "Inventory/Core/CorrbolgInventoryComponent.h"
#include "Items/Definitions/Fragments/CorrbolgInventoryFragment.h"

#include "Actions/CorrbolgActionContextFragments.h"

void UCorrbolgStoreItem::Client_PerformAction_Implementation()
{
	Super::Client_PerformAction_Implementation();
	
	ensureAlwaysMsgf(false, TEXT("The action StoreItem should not be executed on the client!"));

	Finish(ECorrbolgActionResult::Failure);
}

void UCorrbolgStoreItem::Server_PerformAction_Implementation()
{
	Super::Server_PerformAction_Implementation();

	UCorrbolgInventoryComponent* const InventoryComponent = Cast<UCorrbolgInventoryComponent>(Context.InventoryComponent);
	if (!ensureMsgf(InventoryComponent, TEXT("Trying to store an item but the InventoryComponent in the context was not valid!")))
	{
		Finish(ECorrbolgActionResult::Failure);
		return;
	}

	const FCorrbolgStorageContextFragment* const StorageFragment = Context.Payload.GetPtr<FCorrbolgStorageContextFragment>();
	if (!ensureMsgf(StorageFragment != nullptr, TEXT("Trying to store an item but the payload was not valid for this action!")))
	{
		Finish(ECorrbolgActionResult::Failure);
		return;
	}
	
	const UCorrbolgItemDefinition* const ItemDefinition = StorageFragment->Item;
	if (!ensureMsgf(ItemDefinition != nullptr, TEXT("Trying to store an item but the StorageFragment has no item definition!")))
	{
		Finish(ECorrbolgActionResult::Failure);
		return;
	}
	
	const UCorrbolgInventoryFragment* const InventoryFragment = StorageFragment->Item->FindFragmentOfClass<UCorrbolgInventoryFragment>();
	if (!ensureMsgf(InventoryFragment != nullptr, TEXT("Trying to store an item but the item has no Inventory Fragment!")))
	{
		Finish(ECorrbolgActionResult::Failure);
		return;
	}
	
	// If the item is stackable, check if there is an entry and increase the stack size.
	int RemainingStackSize = StorageFragment->StackSize;
	
	if (InventoryFragment->GetIsStackable())
	{
		for (FCorrbolgInventoryEntry& Entry : InventoryComponent->GetStoredEntries())
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
	
	// If there is still some of the stack left, add a new entry to the inventory until everything is stored.
	while(RemainingStackSize > 0)
	{
		const FGuid ItemId = ItemDefinition->GetId();
		const FPrimaryAssetId AssetId = ItemDefinition->GetPrimaryAssetId();
		
		const bool bHasStackLimit = InventoryFragment->GetRecommendedMaxStackSize() > 0; // Is there a stack limit?
		const int MaxStackSize = InventoryFragment->GetRecommendedMaxStackSize();
		const int AmountToAddToStack = bHasStackLimit ? FMath::Min(MaxStackSize, RemainingStackSize) : RemainingStackSize;
		RemainingStackSize -= AmountToAddToStack;

		FCorrbolgInventoryEntry EntryData = FCorrbolgInventoryEntry(ItemId, AssetId, AmountToAddToStack);
		
		// Are there any invalid entries to overwrite?
		FCorrbolgInventoryEntry* const AvailableEntry = InventoryComponent->GetStoredEntries().FindByPredicate([](const FCorrbolgInventoryEntry& Entry)
		{
			return !Entry.IsValid();
		});
	
		if (AvailableEntry)
		{
			*AvailableEntry = EntryData;
		}
		else
		{
			// TODO: Koen: This can cause the inventory to grow indefinitely, we should have a max size and reject storing if there is no space left.
			InventoryComponent->GetStoredEntries().Add(EntryData);
		}
	}

	Finish(ECorrbolgActionResult::Success);
}