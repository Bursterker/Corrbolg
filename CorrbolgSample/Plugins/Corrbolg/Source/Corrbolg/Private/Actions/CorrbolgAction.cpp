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

	// TODO: If each action remains instanced, then the callback needs to be removed. Otherwise a callback is added each time the setup is called and will be executed multiple times.
	OnActionFinished.AddLambda(ActionContext.Callback);
}
