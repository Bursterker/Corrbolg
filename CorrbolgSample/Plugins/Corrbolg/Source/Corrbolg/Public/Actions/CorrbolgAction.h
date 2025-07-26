#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Actions/CorrbolgActionTypes.h"

#include "CorrbolgAction.generated.h"

/**
* Base Interface to perform an action on the CorrbolgInventory.
*/
UCLASS()
class CORRBOLG_API UCorrbolgAction : public UObject
{
	GENERATED_BODY()
	
public:
	/** Executes the defined behavior. */
	UFUNCTION(Server, Reliable, Category = "Action")
	virtual void Execute_Server(const FActionContext& ActionContext);
	virtual void Execute_Server_Implementation(const FActionContext& ActionContext);

protected:
	/** The context the action was called for. */
	FActionContext Context = FActionContext();
};
