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
	virtual void Client_PerformAction_Implementation() override;
	virtual void Server_PerformAction_Implementation() override;

	FCorrbolgInventorySaveGameData GatherSaveData() const;
};
