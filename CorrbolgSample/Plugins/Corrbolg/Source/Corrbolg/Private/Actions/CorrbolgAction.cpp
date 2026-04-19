#include "Actions/CorrbolgAction.h"

#pragma region Core
void UCorrbolgAction::Initialize(UActorComponent* const InventoryComponent, const FInstancedStruct& InPayload, TFunction<void(ECorrbolgActionResult)> Callback)
{
	Context.InventoryComponent = InventoryComponent;
	Context.Payload = InPayload;

	ActionState = EActionState::ReadyToStart;

	OnActionFinished.AddLambda(Callback);
}

void UCorrbolgAction::Execute()
{
	if (IsAuthorative())
	{
		// Server Logic
		Server_PerformAction();
	}
	else
	{
		// Client Logic
		Client_PerformAction();
	}
}

void UCorrbolgAction::Finish(const ECorrbolgActionResult Result)
{
	ActionState = EActionState::Finished;

	OnActionFinished.Broadcast(Result);
	OnActionFinished.Clear();
}
#pragma endregion

#pragma region Replication
bool UCorrbolgAction::IsAuthorative() const
{
	return IsValid(Context.InventoryComponent) && IsValid(Context.InventoryComponent->GetOwner()) && Context.InventoryComponent->GetOwner()->HasAuthority();
}
#pragma endregion

#pragma region Behavior
void UCorrbolgAction::Server_PerformAction_Implementation()
{
	ActionState = EActionState::InProgress;
}

void UCorrbolgAction::Client_PerformAction_Implementation()
{
	ActionState = EActionState::InProgress;
}
#pragma endregion
