#include "Actions/CorrbolgLoadDataFromClient.h"

#include "Kismet/GameplayStatics.h"

#include "Actions/CorrbolgActionContextFragments.h"
#include "Inventory/Core/CorrbolgInventoryComponent.h"
#include "Inventory/SaveGame/CorrbolgInventorySaveGame.h"
#include "Inventory/Definitions/CorrbolgInventoryDefinitions.h"
#include "Items/Definitions/CorrbolgItemTableRow.h"

void UCorrbolgLoadDataFromClient::Client_PerformAction_Implementation()
{
	Super::Client_PerformAction_Implementation();

	ensureAlwaysMsgf(false, TEXT("The action LoadDataFromClient should not be executed on the client!"));

	Finish(ECorrbolgActionResult::Failure);
}

void UCorrbolgLoadDataFromClient::Server_PerformAction_Implementation()
{
	Super::Server_PerformAction_Implementation();

	// The save game data is only available on the client.
	Client_GatherSaveData();
}

void UCorrbolgLoadDataFromClient::Client_GatherSaveData_Implementation()
{
	// Load the SaveGame instance from the local save file.
	UCorrbolgInventorySaveGame* const SaveGameInstance =
		Cast<UCorrbolgInventorySaveGame>(
			UGameplayStatics::LoadGameFromSlot(CorrbolgSaveGame::SaveSlotName, CorrbolgSaveGame::SaveUserIndex));
	
	FCorrbolgInventorySaveGameData SaveGameData = FCorrbolgInventorySaveGameData();
	
	// The SaveGame instance is valid, otherwise provide the server with empty data. (ex: first time playing, deleted save file, etc.)
	if (IsValid(SaveGameInstance))
	{
		// Fill the SaveGame data with the data from the loaded SaveGame instance.
		SaveGameData.SavedInventoryEntries = SaveGameInstance->SaveGameData.SavedInventoryEntries;
	}
	
	Server_ReceiveSaveData(SaveGameData);
}

void UCorrbolgLoadDataFromClient::Server_ReceiveSaveData_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData)
{
	UCorrbolgInventoryComponent* const InventoryComponent = Cast<UCorrbolgInventoryComponent>(Context.InventoryComponent);
	if (!ensureMsgf(InventoryComponent, TEXT("Trying to store an item but the InventoryComponent in the context was not valid!")))
	{
		Finish(ECorrbolgActionResult::Failure);
		return;
	}

	// TODO: Koen: There can be a local Items Table for quick item info like ui, but the server should always use its own Item Table not what is given by the client.
	const FCorrbolgSaveGameContextFragment* const SaveGameFragment = Context.Payload.GetPtr<FCorrbolgSaveGameContextFragment>();
	if (!ensureMsgf(SaveGameFragment != nullptr, TEXT("Trying to Load items but the payload was not valid for this action!")))
	{
		Finish(ECorrbolgActionResult::Failure);
		return;
	}
	
	TArray<FCorrbolgInventoryEntry> LoadedEntries;
	
	for (const FCorrbolgInventoryEntrySaveGameData& SavedEntry : SaveGameData.SavedInventoryEntries)
	{
		FString ContextString = FString();
		FCorrbolgItemTableRow* const TableRow = SaveGameFragment->ItemTable->FindRow<FCorrbolgItemTableRow>(FName(SavedEntry.ObjectId.ToString()), ContextString);
	
		if (!ensureMsgf(TableRow != nullptr, TEXT("Trying to Load items but no entry is found for %s!"), *SavedEntry.ObjectId.ToString()))
		{
			Finish(ECorrbolgActionResult::Failure);
			return;
		}
	
		if (!ensureMsgf(TableRow->ItemDefinition.IsValid(), TEXT("Trying to Load items but entry %s has no ItemDefinition!"), *SavedEntry.ObjectId.ToString()))
		{
			Finish(ECorrbolgActionResult::Failure);
			return;
		}
	
		FPrimaryAssetId AssetId = TableRow->ItemDefinition->GetPrimaryAssetId();
	
		FCorrbolgInventoryEntry Entry = FCorrbolgInventoryEntry(SavedEntry.ObjectId, AssetId, SavedEntry.StackSize);
		LoadedEntries.Add(Entry);
	}
	
	InventoryComponent->GetStoredEntries() = LoadedEntries;

	Finish(ECorrbolgActionResult::Success);
}
