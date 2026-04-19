#include "Actions/CorrbolgPrintDataToLog.h"

#include "Engine/AssetManager.h"

#include "Actions/CorrbolgActionContextFragments.h"
#include "Inventory/Core/CorrbolgInventoryComponent.h"
#include "Inventory/Definitions/CorrbolgInventoryDefinitions.h"
#include "Items/Definitions/CorrbolgItemDefinition.h"
#include "Items/Definitions/Fragments/CorrbolgUIFragment.h"

void UCorrbolgPrintDataToLog::Client_PerformAction_Implementation()
{
	Super::Client_PerformAction_Implementation();

	// Print the data to the client log.
	PrintInventoryEntries();

	// No Server request is made, finish immediately.
	Finish(ECorrbolgActionResult::Success);
}

void UCorrbolgPrintDataToLog::Server_PerformAction_Implementation()
{
	Super::Server_PerformAction_Implementation();

	// Print the data to the server log.
	PrintInventoryEntries();

	// Print the data to the client log.
	Client_PerformAction();

	// No need to wait for the client to confirm the log has been printed.
	Finish(ECorrbolgActionResult::Success);
}

void UCorrbolgPrintDataToLog::PrintInventoryEntries() const
{
	const UCorrbolgInventoryComponent* const InventoryComponent = Cast<UCorrbolgInventoryComponent>(Context.InventoryComponent);
	if (!ensureMsgf(InventoryComponent, TEXT("Trying to log inventory entries but the InventoryComponent in the context was not valid!")))
	{
		return;
	}

	const FCorrbolgLogContextFragment* const LogFragment = Context.Payload.GetPtr<FCorrbolgLogContextFragment>();
	ensureMsgf(LogFragment != nullptr, TEXT("Trying to log but the payload was not valid for this action, logging with reduced data!"));

	UAssetManager& AssetManager = UAssetManager::Get();
	FString EntriesInfo = "";

	// Get the Name and StackSize of each entry.
	for (const FCorrbolgInventoryEntry& Entry : InventoryComponent->GetStoredEntries())
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
}
