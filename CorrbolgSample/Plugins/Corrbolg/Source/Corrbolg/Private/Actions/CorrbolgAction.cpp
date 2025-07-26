#include "Actions/CorrbolgAction.h"

void UCorrbolgAction::Execute_Server_Implementation(const FActionContext& ActionContext)
{
	Context = ActionContext;

	UE_LOG(LogTemp, Log, TEXT("Execute order 66!"));
}
