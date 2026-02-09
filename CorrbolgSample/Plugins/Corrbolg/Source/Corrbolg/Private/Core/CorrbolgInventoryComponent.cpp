#include "Core/CorrbolgInventoryComponent.h"

#include "Net/UnrealNetwork.h"

#include "Actions/CorrbolgActionTypes.h"
#include "Actions//CorrbolgAction.h"
#include "Core/CorrbolgInventorySettings.h"

UCorrbolgInventoryComponent::UCorrbolgInventoryComponent()
{
	SetIsReplicatedByDefault(true);

	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UCorrbolgInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsAuthorative())
	{
		InitializeActions();
	}
}

#pragma region Replication
void UCorrbolgInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCorrbolgInventoryComponent, StoredEntries);
}

bool UCorrbolgInventoryComponent::IsAuthorative() const
{
	return IsValid(GetOwner()) && GetOwner()->HasAuthority();
}
#pragma endregion

#pragma region Actions
void UCorrbolgInventoryComponent::ExecuteAction_Client(const FGameplayTag& ActionId, const FInstancedStruct& Payload)
{
	ExecuteAction_Server(ActionId, Payload);
}

void UCorrbolgInventoryComponent::ExecuteAction_Server_Implementation(const FGameplayTag& ActionId, const FInstancedStruct& Payload)
{
	// Verify the call is on the server/authorative client.
	if (!IsAuthorative() || (ActiveAction != nullptr))
	{
		return;
	}

	ActiveAction = ActionInstanceMap.FindRef(ActionId);
	if (!ensureMsgf(IsValid(ActiveAction), TEXT("Could not find the action in the action map!")))
	{
		return;
	}

	FCorrbolgActionContext Context = FCorrbolgActionContext();
	Context.Owner = this;
	Context.Inventory = &StoredEntries;
	Context.Payload = Payload;
	Context.Callback = [this](const ECorrbolgActionResult Result){this->OnActionExecutionFinished(Result); };

	ActiveAction->Execute_Server(Context);
}

void UCorrbolgInventoryComponent::OnActionExecutionFinished(const ECorrbolgActionResult Result)
{
	UE_LOG(LogTemp, Log, TEXT("Executed order 66 succesfully!"));

	ActiveAction = nullptr;
}

void UCorrbolgInventoryComponent::InitializeActions()
{
	if (!ensureMsgf(IsValid(InventorySettings), TEXT("Missing inventory settings on: %s!"), *GetName()))
	{
		return;
	}

	ActionInstanceMap.Reserve(InventorySettings->ActionMap.Num());

	for (const auto& ActionMapping : InventorySettings->ActionMap)
	{
		UCorrbolgAction* const ActionInstance = NewObject<UCorrbolgAction>(this, ActionMapping.Value.LoadSynchronous());
		if (!ensureMsgf(IsValid(ActionInstance), TEXT("Could not create an Action Instance for %s"), *ActionMapping.Key.ToString()))
		{
			continue;
		}

		ActionInstanceMap.Add(ActionMapping.Key, ActionInstance);
	};
}
#pragma endregion
