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

	SaveGameData.SavedStoredItems = SaveGameInstance->SaveGameData.SavedStoredItems;

	OnSaveDataReceived_Server(SaveGameData);
}

void UCorrbolgLoadDataFromClient::LoadInventory_Server_Implementation() const
{
	LoadInventory_Client();
}

void UCorrbolgLoadDataFromClient::OnSaveDataReceived_Server_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData) const
{
	// TODO: broadcast failure if there was no savegame instance in LoadInventory_Client.
	*Context.StoredItems = SaveGameData.SavedStoredItems;

	OnActionFinished.Broadcast(ECorrbolgActionResult::Success);
}