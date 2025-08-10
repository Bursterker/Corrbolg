#include "Actions/CorrbolgSaveDataOnClient.h"

#include "Kismet/GameplayStatics.h"

#include "SaveGame/CorrbolgInventorySaveGame.h"

void UCorrbolgSaveDataOnClient::Execute_Server_Implementation(const FCorrbolgActionContext& ActionContext)
{
	Super::Execute_Server_Implementation(ActionContext);

	SaveInventory_Server_Implementation();
}

void UCorrbolgSaveDataOnClient::SaveInventory_Client_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData) const
{
	UCorrbolgInventorySaveGame* const SaveGameInstance =
		Cast<UCorrbolgInventorySaveGame>(
			UGameplayStatics::CreateSaveGameObject(UCorrbolgInventorySaveGame::StaticClass()));

	SaveGameInstance->SavedStoredItems = SaveGameData.SavedStoredItems;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, CorrbolgSaveGame::SaveSlotName, CorrbolgSaveGame::SaveUserIndex);
}

void UCorrbolgSaveDataOnClient::SaveInventory_Server_Implementation() const
{
	FCorrbolgInventorySaveGameData SaveData = FCorrbolgInventorySaveGameData();
	SaveData.SavedStoredItems = *Context.StoredItems;

	SaveInventory_Client(SaveData);
}
