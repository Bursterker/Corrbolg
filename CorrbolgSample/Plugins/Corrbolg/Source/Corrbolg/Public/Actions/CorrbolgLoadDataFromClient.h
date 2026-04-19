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
	virtual void Client_PerformAction_Implementation() override;
	virtual void Server_PerformAction_Implementation() override;

	UFUNCTION(Client, Reliable)
	void Client_GatherSaveData();

	UFUNCTION(Server, Reliable)
	void Server_ReceiveSaveData(const FCorrbolgInventorySaveGameData& SaveGameData);
};
