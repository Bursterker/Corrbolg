#pragma once

#include "CoreMinimal.h"
#include "Actions/CorrbolgAction.h"

#include "CorrbolgStoreItem.generated.h"

/**
* Action to store an item into the Corrbolg Inventory.
*/
UCLASS()
class CORRBOLG_API UCorrbolgStoreItem : public UCorrbolgAction
{
	GENERATED_BODY()
	
protected:
	virtual void Client_PerformAction_Implementation() override;

	virtual void Server_PerformAction_Implementation() override;
};
