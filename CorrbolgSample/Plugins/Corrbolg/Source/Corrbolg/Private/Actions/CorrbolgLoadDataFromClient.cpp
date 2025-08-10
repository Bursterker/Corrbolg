#include "Actions/CorrbolgLoadDataFromClient.h"

#include "Kismet/GameplayStatics.h"

#include "SaveGame/CorrbolgInventorySaveGame.h"

ECorrbolgActionResult UCorrbolgLoadDataFromClient::PerformAction_Server(const FCorrbolgActionContext& ActionContext) const
{
	LoadInventory_Server();

	return ECorrbolgActionResult::Success;
}

void UCorrbolgLoadDataFromClient::LoadInventory_Client_Implementation() const
{
	UCorrbolgInventorySaveGame* const SaveGameInstance =
		Cast<UCorrbolgInventorySaveGame>(
			UGameplayStatics::LoadGameFromSlot(CorrbolgSaveGame::SaveSlotName, CorrbolgSaveGame::SaveUserIndex));

	if (!IsValid(SaveGameInstance))
	{
		return;
	}

	FCorrbolgInventorySaveGameData SaveGameData = FCorrbolgInventorySaveGameData();
	SaveGameData.SavedStoredItems = SaveGameInstance->SavedStoredItems;

	OnSaveDataReceived_Server(SaveGameData);
}

void UCorrbolgLoadDataFromClient::LoadInventory_Server_Implementation() const
{
	LoadInventory_Client();
}

void UCorrbolgLoadDataFromClient::OnSaveDataReceived_Server_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData) const
{
	*Context.StoredItems = SaveGameData.SavedStoredItems;
}