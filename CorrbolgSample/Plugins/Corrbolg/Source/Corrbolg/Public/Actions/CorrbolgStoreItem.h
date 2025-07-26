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
	
public:
	virtual void Execute_Server_Implementation(const FActionContext& ActionContext) override;

protected:
	/** Stores an item in the inventory. */
	UFUNCTION(Server, Reliable, Category = "Action|Manipulation")
	virtual void StoreItem_Server() const;
	virtual void StoreItem_Server_Implementation() const;
};
