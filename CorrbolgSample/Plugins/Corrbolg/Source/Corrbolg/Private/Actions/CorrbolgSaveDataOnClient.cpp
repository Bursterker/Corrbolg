#include "Actions/CorrbolgSaveDataOnClient.h"

#include "Kismet/GameplayStatics.h"

#include "Inventory/SaveGame/CorrbolgInventorySaveGame.h"

void UCorrbolgSaveDataOnClient::PerformAction(const FCorrbolgActionContext& ActionContext)
{
	SaveInventory_Server_Implementation();
}

void UCorrbolgSaveDataOnClient::SaveInventory_Client_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData)
{
	UCorrbolgInventorySaveGame* const SaveGameInstance =
		Cast<UCorrbolgInventorySaveGame>(
			UGameplayStatics::CreateSaveGameObject(UCorrbolgInventorySaveGame::StaticClass()));

	SaveGameInstance->SaveGameData.SavedInventoryEntries = SaveGameData.SavedInventoryEntries;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, CorrbolgSaveGame::SaveSlotName, CorrbolgSaveGame::SaveUserIndex);
}

void UCorrbolgSaveDataOnClient::SaveInventory_Server_Implementation()
{
	FCorrbolgInventorySaveGameData SaveData = FCorrbolgInventorySaveGameData();

	for (const FCorrbolgInventoryEntry& Entry : *Context.Inventory)
	{
		if(!Entry.IsValid())
		{
			continue;
		}

		FCorrbolgInventoryEntrySaveGameData SavedEntry = FCorrbolgInventoryEntrySaveGameData();
		SavedEntry.ObjectId = Entry.GetObjectId();
		SavedEntry.StackSize = Entry.GetStackSize();

		SaveData.SavedInventoryEntries.Add(SavedEntry);
	}

	SaveInventory_Client(SaveData);
}
