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

	// TODO: Koen: Don't save the PRimary Asset Id, it needs to be reinitialized for safety between updates as the primary asset id can ahve changed with refactors.
	SaveGameInstance->SaveGameData.SavedInventoryEntries = SaveGameData.SavedInventoryEntries;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, CorrbolgSaveGame::SaveSlotName, CorrbolgSaveGame::SaveUserIndex);
}

void UCorrbolgSaveDataOnClient::SaveInventory_Server_Implementation() const
{
	FCorrbolgInventorySaveGameData SaveData = FCorrbolgInventorySaveGameData();
	SaveData.SavedInventoryEntries = *Context.Inventory;

	SaveInventory_Client(SaveData);
}
