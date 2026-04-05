#include "Actions/CorrbolgAction.h"

void UCorrbolgAction::Execute_Server_Implementation(const FCorrbolgActionContext& ActionContext)
{
	// Setup the action based on the given context.
	SetupAction(ActionContext);

	// Execute the action and notify listeners.
	PerformAction(ActionContext);

	if (!bIsAsyncAction)
	{
		FinishAction(ECorrbolgActionResult::Success);
	}
}

void UCorrbolgAction::SetupAction(const FCorrbolgActionContext& ActionContext)
{
	Context = ActionContext;

	OnActionFinished.AddLambda(ActionContext.Callback);
}

void UCorrbolgAction::FinishAction(const ECorrbolgActionResult Result)
{
	OnActionFinished.Broadcast(Result);
	OnActionFinished.Clear();
}
