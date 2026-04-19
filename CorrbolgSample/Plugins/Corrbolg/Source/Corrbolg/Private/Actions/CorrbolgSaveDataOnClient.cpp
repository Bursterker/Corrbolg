#include "Actions/CorrbolgSaveDataOnClient.h"

#include "Kismet/GameplayStatics.h"

#include "Inventory/Core/CorrbolgInventoryComponent.h"
#include "Inventory/SaveGame/CorrbolgInventorySaveGame.h"

void UCorrbolgSaveDataOnClient::Client_PerformAction_Implementation()
{
	Super::Client_PerformAction_Implementation();

	FCorrbolgInventorySaveGameData SaveGameData = GatherSaveData();

	UCorrbolgInventorySaveGame* const SaveGameInstance =
		Cast<UCorrbolgInventorySaveGame>(
			UGameplayStatics::CreateSaveGameObject(UCorrbolgInventorySaveGame::StaticClass()));
	
	SaveGameInstance->SaveGameData.SavedInventoryEntries = SaveGameData.SavedInventoryEntries;
	

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, CorrbolgSaveGame::SaveSlotName, CorrbolgSaveGame::SaveUserIndex);

	Finish(ECorrbolgActionResult::Success);
}

void UCorrbolgSaveDataOnClient::Server_PerformAction_Implementation()
{
	Super::Server_PerformAction_Implementation();

	Client_PerformAction_Implementation();

	// No need to wait for Client confirmation.
	Finish(ECorrbolgActionResult::Success);
}

FCorrbolgInventorySaveGameData UCorrbolgSaveDataOnClient::GatherSaveData() const
{
	FCorrbolgInventorySaveGameData SaveData = FCorrbolgInventorySaveGameData();

	const UCorrbolgInventoryComponent* const InventoryComponent = Cast<UCorrbolgInventoryComponent>(Context.InventoryComponent);
	if (!ensureMsgf(InventoryComponent, TEXT("Trying to Save Game Data but the InventoryComponent in the context was not valid!")))
	{
		return SaveData;
	}

	
	for (const FCorrbolgInventoryEntry& Entry : InventoryComponent->GetStoredEntries())
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

	return SaveData;
}
