#include "Actions/CorrbolgAction.h"

void UCorrbolgAction::Execute_Server_Implementation(const FCorrbolgActionContext& ActionContext)
{
	// TODO: Fix safety, other actions call this, but should not broadcast nor set result at this point.
	// Remove this method from being virtual, and provide a logic only method to override that get's called from here.
	// Then at the this can keep the broadcast with the result being set by children.
	Context = ActionContext;
	Result = ECorrbolgActionResult::Success;

	OnActionFinished.AddLambda(ActionContext.Callback);
	OnActionFinished.Broadcast(Result);
}
