#include "Actions/CorrbolgLoadDataFromClient.h"

#include "Kismet/GameplayStatics.h"

#include "SaveGame/CorrbolgInventorySaveGame.h"

void UCorrbolgLoadDataFromClient::Execute_Server_Implementation(const FCorrbolgActionContext& ActionContext)
{
	Super::Execute_Server_Implementation(ActionContext);

	LoadInventory_Server();
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