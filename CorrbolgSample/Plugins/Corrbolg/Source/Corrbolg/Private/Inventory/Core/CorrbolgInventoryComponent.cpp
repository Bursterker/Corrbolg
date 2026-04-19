#include "Inventory/Core/CorrbolgInventoryComponent.h"

#include "Net/UnrealNetwork.h"

#include "Actions/CorrbolgActionTypes.h"
#include "Actions//CorrbolgAction.h"
#include "Inventory/Core/CorrbolgInventorySettings.h"

UCorrbolgInventoryComponent::UCorrbolgInventoryComponent()
{
	SetIsReplicatedByDefault(true);

	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UCorrbolgInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

#pragma region Replication
void UCorrbolgInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCorrbolgInventoryComponent, StoredEntries);
}
#pragma endregion

#pragma region Actions
void UCorrbolgInventoryComponent::RequestAction(const FGameplayTag& ActionId, const FInstancedStruct& Payload)
{
	TSoftClassPtr<UCorrbolgAction>* const ActionTemplate = GetActionTemplate(ActionId);
	if (!ensureMsgf(ActionTemplate, TEXT("Action template not found for ActionId: %s"), *ActionId.ToString()))
	{
		return;
	}

	UCorrbolgAction* const ActionInstance = NewObject<UCorrbolgAction>(this, ActionTemplate->LoadSynchronous());
	if (!ensureMsgf(ActionInstance, TEXT("Failed to create action instance for ActionId: %s"), *ActionId.ToString()))
	{
		return;
	}

	// Put the action in the queue and initialize it with the payload. An action called from the Client, is responsible for creating a request on the server.
	ActionInstance->Initialize(this, Payload, [this](const ECorrbolgActionResult Result) { this->ProcessActionQueue(); });
	ActionQueue.Enqueue(ActionInstance);

	ProcessActionQueue();
}

void UCorrbolgInventoryComponent::ProcessActionQueue()
{
	TObjectPtr<UCorrbolgAction>* const ActingActionPtr = ActionQueue.Peek();
	if (!ActingActionPtr)
	{
		return;
	}

	UCorrbolgAction* const ActingAction = *ActingActionPtr;
	switch (ActingAction->GetActionState())
	{
		case EActionState::ReadyToStart:
		{
			UE_LOG(LogTemp, Log, TEXT("Action starting: %s"), *ActingAction->GetName());
			ActingAction->Execute();
			
			break;
		}
		case EActionState::Finished:
		{
			ActionQueue.Pop();
			UE_LOG(LogTemp, Log, TEXT("Action finished and removed from queue: %s"), *ActingAction->GetName());

			ProcessActionQueue();
			break;
		}
		default:
			break;
	}
}

TSoftClassPtr<UCorrbolgAction>* UCorrbolgInventoryComponent::GetActionTemplate(const FGameplayTag& ActionId)
{
	return InventorySettings->ActionMap.Find(ActionId);
}
#pragma endregion
