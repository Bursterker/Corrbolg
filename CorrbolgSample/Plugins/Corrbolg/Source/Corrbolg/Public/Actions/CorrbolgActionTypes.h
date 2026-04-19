#pragma once
#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"

#include "Inventory/Definitions/CorrbolgInventoryDefinitions.h"

#include "CorrbolgActionTypes.generated.h"

UENUM()
enum class EActionState
{
	Invalid,
	ReadyToStart,
	InProgress,
	Finished
};

/*
* Result of an action that was executed.
*/
UENUM(BlueprintType)
enum class ECorrbolgActionResult : uint8
{
	Success,	// Action was executed without issues.
	Failure,	// Action could not be succesfully executed.
	None		// Default State.
};

/*
* Context for CorrbolgActions to provide information on what to execute an action on.
*/
USTRUCT(BlueprintType)
struct FCorrbolgActionContext
{
	GENERATED_BODY()

public:
	/** The inventory component being handled. */
	UPROPERTY(Transient)
	TObjectPtr<UActorComponent> InventoryComponent = nullptr;

	/** Payload data given when requesting the action. */
	UPROPERTY(Transient)
	FInstancedStruct Payload;
};
