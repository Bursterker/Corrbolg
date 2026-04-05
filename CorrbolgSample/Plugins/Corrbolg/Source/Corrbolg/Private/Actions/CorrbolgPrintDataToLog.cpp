#include "Actions/CorrbolgPrintDataToLog.h"

#include "Engine/AssetManager.h"

#include "Actions/CorrbolgActionContextFragments.h"
#include "Inventory/Definitions/CorrbolgInventoryDefinitions.h"
#include "Items/Definitions/CorrbolgItemDefinition.h"
#include "Items/Definitions/Fragments/CorrbolgUIFragment.h"

void UCorrbolgPrintDataToLog::PerformAction(const FCorrbolgActionContext& ActionContext)
{
	const FCorrbolgLogContextFragment* const LogFragment = Context.Payload.GetPtr<FCorrbolgLogContextFragment>();
	ensureMsgf(LogFragment != nullptr, TEXT("Trying to log but the payload was not valid for this action, logging with reduced data!"));

	UAssetManager& AssetManager = UAssetManager::Get();

	FString EntriesInfo = "";

	// Get the Name and StackSize of each entry.
	for (const FCorrbolgInventoryEntry& Entry : *Context.Inventory)
	{
		if (!Entry.IsValid())
		{
			EntriesInfo.Append("\nInvalid Entry");
			continue;
		}
		
		const FPrimaryAssetId& AssetId = Entry.GetAssetId();
		AssetManager.LoadPrimaryAsset(AssetId);

		const UCorrbolgItemDefinition* const Item = AssetManager.GetPrimaryAssetObject<UCorrbolgItemDefinition>(AssetId);
		if (!Item)
		{
			continue;
		}

		// What is the name of the item?
		FString ItemName = Item->GetName();

		// Overwrite with the UI name if possible.
		const UCorrbolgUIFragment* const UIFragment = Item->FindFragmentOfClass<UCorrbolgUIFragment>();
		if (UIFragment && LogFragment)
		{
			ItemName = UIFragment->FindText(LogFragment->UINameTag)->ToString();
		}

		EntriesInfo.Append(FString::Format(TEXT("\nItem: {0}, StackSize: {1}"), { ItemName, Entry.GetStackSize()}));
	}

	UE_LOG(LogTemp, Log, TEXT("Stored Items : %s"), *EntriesInfo);
	
	FinishAction(ECorrbolgActionResult::Success);
}
