#include "Actions/CorrbolgAction.h"

void UCorrbolgAction::Execute_Server_Implementation(const FCorrbolgActionContext& ActionContext)
{
	// Setup the action based on the given context.
	SetupAction(ActionContext);

	// Execute the action and notify listeners.
	PerformAction(ActionContext);
}

void UCorrbolgAction::SetupAction(const FCorrbolgActionContext& ActionContext)
{
	Context = ActionContext;
	OnActionFinished.AddLambda(ActionContext.Callback);
}
