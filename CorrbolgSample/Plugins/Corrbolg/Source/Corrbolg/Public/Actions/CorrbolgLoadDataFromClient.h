#pragma once

#include "CoreMinimal.h"
#include "Actions/CorrbolgAction.h"

#include "CorrbolgLoadDataFromClient.generated.h"

struct FCorrbolgInventorySaveGameData;

/**
* Action to load saved data into the inventory from the clients save file.
*/
UCLASS()
class CORRBOLG_API UCorrbolgLoadDataFromClient : public UCorrbolgAction
{
	GENERATED_BODY()

protected:
	virtual void SetupAction(const FCorrbolgActionContext& ActionContext) override;
	virtual void PerformAction(const FCorrbolgActionContext& ActionContext) override;

	/** Reads the inventory data from the save game and sends it to the server for replication. */
	UFUNCTION(Client, Reliable, Category = "Action|SaveData")
	virtual void LoadInventory_Client();
	virtual void LoadInventory_Client_Implementation();

	/** Request the client for the local save game data. */
	UFUNCTION(Server, Reliable, Category = "Action|SaveData")
	virtual void LoadInventory_Server();
	virtual void LoadInventory_Server_Implementation();

	/** Replicates the received local save game to all clients. */
	UFUNCTION(Server, Reliable, Category = "Action|SaveData")
	virtual void OnSaveDataReceived_Server(const FCorrbolgInventorySaveGameData& SaveGameData);
	virtual void OnSaveDataReceived_Server_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData);

};
