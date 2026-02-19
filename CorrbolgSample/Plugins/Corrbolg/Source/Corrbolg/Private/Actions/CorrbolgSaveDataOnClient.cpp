#include "Actions/CorrbolgSaveDataOnClient.h"

#include "Kismet/GameplayStatics.h"

#include "Inventory/SaveGame/CorrbolgInventorySaveGame.h"

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

	SaveGameInstance->SaveGameData.SavedInventoryEntries = SaveGameData.SavedInventoryEntries;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, CorrbolgSaveGame::SaveSlotName, CorrbolgSaveGame::SaveUserIndex);
}

void UCorrbolgSaveDataOnClient::SaveInventory_Server_Implementation() const
{
	FCorrbolgInventorySaveGameData SaveData = FCorrbolgInventorySaveGameData();

	for (const FCorrbolgInventoryEntry& Entry : *Context.Inventory)
	{
		FCorrbolgInventoryEntrySaveGameData SavedEntry = FCorrbolgInventoryEntrySaveGameData();
		SavedEntry.ObjectId = Entry.GetObjectId();
		SavedEntry.StackSize = Entry.GetStackSize();

		SaveData.SavedInventoryEntries.Add(SavedEntry);
	}

	SaveInventory_Client(SaveData);
}
