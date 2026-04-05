#include "Actions/CorrbolgLoadDataFromClient.h"

#include "Kismet/GameplayStatics.h"

#include "Actions/CorrbolgActionContextFragments.h"
#include "Inventory/SaveGame/CorrbolgInventorySaveGame.h"
#include "Inventory/Definitions/CorrbolgInventoryDefinitions.h"
#include "Items/Definitions/CorrbolgItemTableRow.h"

void UCorrbolgLoadDataFromClient::SetupAction(const FCorrbolgActionContext& ActionContext)
{
	Super::SetupAction(ActionContext);

	bIsAsyncAction = true;
}

void UCorrbolgLoadDataFromClient::PerformAction(const FCorrbolgActionContext& ActionContext)
{
	LoadInventory_Server();
}

void UCorrbolgLoadDataFromClient::LoadInventory_Client_Implementation()
{
	// Load the SaveGame instance from the local save file.
	UCorrbolgInventorySaveGame* const SaveGameInstance =
		Cast<UCorrbolgInventorySaveGame>(
			UGameplayStatics::LoadGameFromSlot(CorrbolgSaveGame::SaveSlotName, CorrbolgSaveGame::SaveUserIndex));

	FCorrbolgInventorySaveGameData SaveGameData = FCorrbolgInventorySaveGameData();

	// The SaveGame instance is invalid, provide the server with empty data. (ex: first time playing, deleted save file, etc.)
	if (!IsValid(SaveGameInstance))
	{
		OnSaveDataReceived_Server(SaveGameData);
		return;
	}

	// Fill the SaveGame data with the data from the loaded SaveGame instance and send it to the server for replication.
	SaveGameData.SavedInventoryEntries = SaveGameInstance->SaveGameData.SavedInventoryEntries;

	OnSaveDataReceived_Server(SaveGameData);
}

void UCorrbolgLoadDataFromClient::LoadInventory_Server_Implementation()
{
	// The SaveGame data is only available on the client, so we need to call a client rpc to get the data and send it to the server for replication.
	LoadInventory_Client();
}

void UCorrbolgLoadDataFromClient::OnSaveDataReceived_Server_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData)
{
	// TODO: Koen: There can be a local Items Table for quick item info like ui, but the server should always use its own Item Table not what is given by the client.
	const FCorrbolgSaveGameContextFragment* const SaveGameFragment = Context.Payload.GetPtr<FCorrbolgSaveGameContextFragment>();
	if (!ensureMsgf(SaveGameFragment != nullptr, TEXT("Trying to Load items but the payload was not valid for this action!")))
	{
		FinishAction(ECorrbolgActionResult::Failure);
		return;
	}

	TArray<FCorrbolgInventoryEntry> LoadedEntries;

	for (const FCorrbolgInventoryEntrySaveGameData& SavedEntry : SaveGameData.SavedInventoryEntries)
	{
		FString ContextString = FString();
		FCorrbolgItemTableRow* const TableRow = SaveGameFragment->ItemTable->FindRow<FCorrbolgItemTableRow>(FName(SavedEntry.ObjectId.ToString()), ContextString);

		if (!ensureMsgf(TableRow != nullptr, TEXT("Trying to Load items but no entry is found for %s!"), *SavedEntry.ObjectId.ToString()))
		{
			FinishAction(ECorrbolgActionResult::Failure);
			return;
		}

		if (!ensureMsgf(TableRow->ItemDefinition.IsValid(), TEXT("Trying to Load items but entry %s has no ItemDefinition!"), *SavedEntry.ObjectId.ToString()))
		{
			FinishAction(ECorrbolgActionResult::Failure);
			return;
		}

		FPrimaryAssetId AssetId = TableRow->ItemDefinition->GetPrimaryAssetId();

		FCorrbolgInventoryEntry Entry = FCorrbolgInventoryEntry(SavedEntry.ObjectId, AssetId, SavedEntry.StackSize);
		LoadedEntries.Add(Entry);
	}

	Context.Inventory->Append(LoadedEntries);

	FinishAction(ECorrbolgActionResult::Success);
}