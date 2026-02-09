#include "Actions/CorrbolgLoadDataFromClient.h"

#include "Kismet/GameplayStatics.h"

#include "SaveGame/CorrbolgInventorySaveGame.h"

void UCorrbolgLoadDataFromClient::PerformAction(const FCorrbolgActionContext& ActionContext) const
{
	LoadInventory_Server();
}

void UCorrbolgLoadDataFromClient::LoadInventory_Client_Implementation() const
{
	UCorrbolgInventorySaveGame* const SaveGameInstance =
		Cast<UCorrbolgInventorySaveGame>(
			UGameplayStatics::LoadGameFromSlot(CorrbolgSaveGame::SaveSlotName, CorrbolgSaveGame::SaveUserIndex));

	FCorrbolgInventorySaveGameData SaveGameData = FCorrbolgInventorySaveGameData();

	if (!IsValid(SaveGameInstance))
	{
		OnSaveDataReceived_Server(SaveGameData);
		return;
	}

	SaveGameData.SavedInventoryEntries = SaveGameInstance->SaveGameData.SavedInventoryEntries;

	OnSaveDataReceived_Server(SaveGameData);
}

void UCorrbolgLoadDataFromClient::LoadInventory_Server_Implementation() const
{
	LoadInventory_Client();
}

void UCorrbolgLoadDataFromClient::OnSaveDataReceived_Server_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData) const
{
	for (const FCorrbolgInventoryEntrySaveGameData& SavedEntry : SaveGameData.SavedInventoryEntries)
	{
		// TODO: Koen: Lookup the Asset ID based on the ObjectId.
		FPrimaryAssetId AssetId;

		FCorrbolgInventoryEntry Entry = FCorrbolgInventoryEntry(SavedEntry.ObjectId, AssetId,SavedEntry.StackSize);
		Context.Inventory->Add(Entry);
	}

	OnActionFinished.Broadcast(ECorrbolgActionResult::Success);
}