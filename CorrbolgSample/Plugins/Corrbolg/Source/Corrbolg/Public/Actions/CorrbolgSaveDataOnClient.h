#pragma once

#include "CoreMinimal.h"
#include "Actions/CorrbolgAction.h"

#include "CorrbolgSaveDataOnClient.generated.h"

enum class ECorrbolgActionResult : uint8;

struct FCorrbolgInventorySaveGameData;

/**
* Action to save data of the current inventory to the client save file.
*/
UCLASS()
class CORRBOLG_API UCorrbolgSaveDataOnClient : public UCorrbolgAction
{
	GENERATED_BODY()
	
protected:
	virtual void PerformAction(const FCorrbolgActionContext& ActionContext) const override;

	/** Writes the inventory data to the save game. */
	UFUNCTION(Client, Reliable, Category = "Action|SaveData")
	virtual void SaveInventory_Client(const FCorrbolgInventorySaveGameData& SaveGameData) const;
	virtual void SaveInventory_Client_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData) const;

	/** Creates the save game object and sends it to the client for local save. */
	UFUNCTION(Server, Reliable, Category = "Action|SaveData")
	virtual void SaveInventory_Server() const;
	virtual void SaveInventory_Server_Implementation() const;

};
