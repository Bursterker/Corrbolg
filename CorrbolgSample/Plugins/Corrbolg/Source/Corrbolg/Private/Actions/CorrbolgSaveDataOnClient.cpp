#include "Actions/CorrbolgSaveDataOnClient.h"

#include "Kismet/GameplayStatics.h"

#include "SaveGame/CorrbolgInventorySaveGame.h"

void UCorrbolgSaveDataOnClient::PerformAction(const FCorrbolgActionContext& ActionContext) const
{
	SaveInventory_Server_Implementation();

	OnActionFinished.Broadcast(ECorrbolgActionResult::Success);
}

void UCorrbolgSaveDataOnClient::SaveInventory_Client_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData) const
{
	UCorrbolgInventorySaveGame* const SaveGameInstance =
		Cast<UCorrbolgInventorySaveGame>(
			UGameplayStatics::CreateSaveGameObject(UCorrbolgInventorySaveGame::StaticClass()));

	SaveGameInstance->SaveGameData.SavedStoredItems = SaveGameData.SavedStoredItems;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, CorrbolgSaveGame::SaveSlotName, CorrbolgSaveGame::SaveUserIndex);
}

void UCorrbolgSaveDataOnClient::SaveInventory_Server_Implementation() const
{
	FCorrbolgInventorySaveGameData SaveData = FCorrbolgInventorySaveGameData();
	SaveData.SavedStoredItems = *Context.StoredItems;

	SaveInventory_Client(SaveData);
}
