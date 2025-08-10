#include "Actions/CorrbolgAction.h"

void UCorrbolgAction::Execute_Server_Implementation(const FCorrbolgActionContext& ActionContext)
{
	// Setup the action based on the given context.
	SetupAction_Server(ActionContext);

	// Execute the action and notify listeners.
	Result = PerformAction_Server(ActionContext);
	OnActionFinished.Broadcast(Result);
}

void UCorrbolgAction::SetupAction_Server(const FCorrbolgActionContext& ActionContext)
{
	Context = ActionContext;
	OnActionFinished.AddLambda(ActionContext.Callback);
}

ECorrbolgActionResult UCorrbolgAction::PerformAction_Server(const FCorrbolgActionContext& ActionContext) const
{
	return ECorrbolgActionResult::Success;
}
