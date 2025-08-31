#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Actions/CorrbolgActionTypes.h"

#include "CorrbolgAction.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FCorrbolgOnActionFinished, const ECorrbolgActionResult);

/**
* Base Interface to perform an action on the CorrbolgInventory.
* Children are required to atleast implement the following:
* - Broadcast OnActionFinished delegate
* - Implement PerformAction_Server
*/
UCLASS(Abstract)
class CORRBOLG_API UCorrbolgAction : public UObject
{
	GENERATED_BODY()
	
public:
	/** Sets up the action and executes it. */
	UFUNCTION(Server, Reliable, Category = "Action")
	void Execute_Server(const FCorrbolgActionContext& ActionContext);
	void Execute_Server_Implementation(const FCorrbolgActionContext& ActionContext);

	/** Delegate fired when the execution of the action has completed. */
	FCorrbolgOnActionFinished OnActionFinished = FCorrbolgOnActionFinished();

protected:
	/** The context the action was called for. */
	FCorrbolgActionContext Context = FCorrbolgActionContext();

	/** Setup required variables for access during execution. */
	virtual void SetupAction(const FCorrbolgActionContext& ActionContext);

	/** Executes the defined behavior. */
	virtual void PerformAction(const FCorrbolgActionContext& ActionContext) const PURE_VIRTUAL(UCorrbolgAction::PerformAction);
};
