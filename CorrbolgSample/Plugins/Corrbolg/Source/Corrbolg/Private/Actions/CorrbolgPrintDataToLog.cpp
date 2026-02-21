#include "Actions/CorrbolgPrintDataToLog.h"

#include "Engine/AssetManager.h"

#include "Inventory/Definitions/CorrbolgInventoryDefinitions.h"
#include "Items/Definitions/CorrbolgItemDefinition.h"
#include "Items/Definitions/Fragments/CorrbolgUIFragment.h"

void UCorrbolgPrintDataToLog::PerformAction(const FCorrbolgActionContext& ActionContext) const
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to print stored inventory entries, but this function is not implemented!"));

	UAssetManager& AssetManager = UAssetManager::Get();

	FString StoredItemsString = "";

	for (const FCorrbolgInventoryEntry& Entry : *Context.Inventory)
	{
		const FPrimaryAssetId& AssetId = Entry.GetAssetId();
		AssetManager.LoadPrimaryAsset(AssetId);

		const UCorrbolgItemDefinition* const Item = AssetManager.GetPrimaryAssetObject<UCorrbolgItemDefinition>(AssetId);
		if (!Item)
		{
			continue;
		}

		const UCorrbolgUIFragment* const UIFragment = Item->FindFragmentOfClass<UCorrbolgUIFragment>();
		if (!UIFragment)
		{
			continue;
		}

		// TODO: Koen: Get the GameplayTag from the payload, then get that text from the uifragment.
		//StoredItemsString.Append(UIFragment->FindText());
		StoredItemsString.Append(Item->GetName());
		StoredItemsString.Append(", ");
	}

	UE_LOG(LogTemp, Log, TEXT("Stored Items : %s"), *StoredItemsString);
	
	OnActionFinished.Broadcast(ECorrbolgActionResult::Success);
}
