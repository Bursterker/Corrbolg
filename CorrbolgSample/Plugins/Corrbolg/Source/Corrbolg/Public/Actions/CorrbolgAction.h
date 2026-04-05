#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Actions/CorrbolgActionTypes.h"

#include "CorrbolgAction.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FCorrbolgOnActionFinished, const ECorrbolgActionResult);

/**
* Base Interface to perform an action on the CorrbolgInventory.
* Children are required to atleast implement the following:
* - Implement PerformAction_Server
* - Call FinishAction if bIsAsyncAction is true.
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

protected:
	/** The context the action was called for. */
	FCorrbolgActionContext Context = FCorrbolgActionContext();

	/** Will this action during execution wait for external execution? */
	bool bIsAsyncAction = false;

	/** Setup required variables for access during execution. */
	virtual void SetupAction(const FCorrbolgActionContext& ActionContext);

	/** Clean up the action making it available for newxt use. */
	virtual void FinishAction(const ECorrbolgActionResult Result);

	/** Executes the defined behavior. */
	virtual void PerformAction(const FCorrbolgActionContext& ActionContext) PURE_VIRTUAL(UCorrbolgAction::PerformAction);

private:
	/** Delegate fired when the execution of the action has completed. */
	FCorrbolgOnActionFinished OnActionFinished = FCorrbolgOnActionFinished();

};
