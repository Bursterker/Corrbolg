#include "Core/CorrbolgInventoryComponent.h"

#include "Net/UnrealNetwork.h"

#include "Actions/CorrbolgAction.h"

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

#pragma region Manipulation
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

	const TSoftClassPtr<UCorrbolgAction>* const SoftActionClass = ActionMapping.Find(Action);
	if (!ensureMsgf(SoftActionClass != nullptr, TEXT("Could not find the action in the action mappings!")))
	{
		return;
	}

	const UClass* const ActionClass = SoftActionClass->LoadSynchronous();
	UCorrbolgAction* const ActionInstance = NewObject<UCorrbolgAction>(GetTransientPackage(), ActionClass);

	// TODO: Generate dynamic data for actions that need it. So not each context has "Item" defined.
	FActionContext Context = FActionContext();
	Context.StoredItems = &StoredItems;
	Context.Item = "Apple";

	ActionInstance->Execute_Server(Context);

	if(Action != ECorrbolgAction::SaveData && Action != ECorrbolgAction::Log)
	{
		ExecuteAction_Server_Implementation(ECorrbolgAction::SaveData);
		ExecuteAction_Server_Implementation(ECorrbolgAction::Log);
	}
}
#pragma endregion
