#pragma once

#include "CoreMinimal.h"

#include "Actions/CorrbolgActionTypes.h"

#include "CorrbolgAction.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FCorrbolgOnActionFinished, const ECorrbolgActionResult);

/**
* Base class to perform an action on the CorrbolgInventory.
* Children are required to integrate the following changes:
* - Override: Client_PerformAction
* - Override: Server_PerformAction
* - Call: Finish
*/
UCLASS(Abstract)
class CORRBOLG_API UCorrbolgAction : public UObject
{
	GENERATED_BODY()

#pragma region Core
public:
	virtual void Initialize(UActorComponent* const InventoryComponent, const FInstancedStruct& InPayload, TFunction<void(ECorrbolgActionResult)> Callback);

	/** Executes behavior based on wether the action is being called from the Client or Server. */
	void Execute();

	/** Returns the current state of the action. */
	EActionState GetActionState() const { return ActionState; }

protected:
	/** The context the action was called for. */
	FCorrbolgActionContext Context = FCorrbolgActionContext();

	virtual void Finish(const ECorrbolgActionResult Result);

private:
	/** The current state of the action. */
	EActionState ActionState = EActionState::Invalid;

	/** Delegate fired when the execution of the action has finished. */
	FCorrbolgOnActionFinished OnActionFinished = FCorrbolgOnActionFinished();
#pragma endregion

#pragma region Replication
public:
	bool IsAuthorative() const;

#pragma endregion

#pragma region Behavior
protected:
	UFUNCTION(Client, Reliable)
	virtual void Client_PerformAction();
	
	UFUNCTION(Server, Reliable)
	virtual void Server_PerformAction();
#pragma endregion
};
