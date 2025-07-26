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
void UCorrbolgInventoryComponent::ExecuteAction_Client(const ECorrbolgAction& Action)
{
	ExecuteAction_Server(Action);
}

void UCorrbolgInventoryComponent::ExecuteAction_Server_Implementation(const ECorrbolgAction& Action)
{
	// Verify the call is on the server/authorative client.
	if (!IsAuthorative())
	{
		return;
	}

	FCorrbolgActionMapping* const ActionMapping = ActionMap.Find(Action);
	if (!ensureMsgf(ActionMapping != nullptr, TEXT("Could not find the action in the action map!")))
	{
		return;
	}

	// TODO: Generate dynamic data for actions that need it. So not each context has "Item" defined.
	FActionContext Context = FActionContext();
	Context.Owner = this;
	Context.StoredItems = &StoredItems;
	Context.Item = "Apple";

	ActionMapping->ExecuteAction(Context);

	if(Action != ECorrbolgAction::SaveData && Action != ECorrbolgAction::Log)
	{
		ExecuteAction_Server_Implementation(ECorrbolgAction::SaveData);
		ExecuteAction_Server_Implementation(ECorrbolgAction::Log);
	}
}
#pragma endregion
