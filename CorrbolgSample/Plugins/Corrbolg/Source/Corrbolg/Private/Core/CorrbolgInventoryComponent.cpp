#include "Core/CorrbolgInventoryComponent.h"

#include "Net/UnrealNetwork.h"

#include "Actions/CorrbolgActionTypes.h"

UCorrbolgInventoryComponent::UCorrbolgInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCorrbolgInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCorrbolgInventoryComponent, StoredItems);
}

void UCorrbolgInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsAuthorative())
	{
		ExecuteAction_Server_Implementation(ECorrbolgAction::LoadData);
	}
}

bool UCorrbolgInventoryComponent::IsAuthorative() const
{
	return IsValid(GetOwner()) && GetOwner()->HasAuthority();
}

#pragma region Actions
void UCorrbolgInventoryComponent::ExecuteAction_Client(const ECorrbolgAction& Action, const FInstancedStruct& Payload)
{
	ExecuteAction_Server(Action, Payload);
}

void UCorrbolgInventoryComponent::ExecuteAction_Server_Implementation(const ECorrbolgAction& Action, const FInstancedStruct& Payload)
{
	// Verify the call is on the server/authorative client.
	if (!IsAuthorative() || bIsHandlingAction)
	{
		return;
	}

	FCorrbolgActionMapping* const ActionMapping = ActionMap.Find(Action);
	if (!ensureMsgf(ActionMapping != nullptr, TEXT("Could not find the action in the action map!")))
	{
		return;
	}

	bIsHandlingAction = true;

	FCorrbolgActionContext Context = FCorrbolgActionContext();
	Context.Owner = this;
	Context.StoredItems = &StoredItems;
	Context.Payload = Payload;
	Context.Callback = [this](const ECorrbolgActionResult Result){this->OnActionExecutionFinished(Result); };

	ActionMapping->ExecuteAction(Context);
}

void UCorrbolgInventoryComponent::OnActionExecutionFinished(const ECorrbolgActionResult Result)
{
	UE_LOG(LogTemp, Log, TEXT("Executed order 66 succesfully!"));

	bIsHandlingAction = false;
}
#pragma endregion
