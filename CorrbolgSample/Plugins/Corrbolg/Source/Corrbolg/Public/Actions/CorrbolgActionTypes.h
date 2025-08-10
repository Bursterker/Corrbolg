#pragma once
#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"

#include "CorrbolgActionTypes.generated.h"

class UCorrbolgAction;

/**
* Actions to execute on the corrbolg inventory.
*/
UENUM(BlueprintType)
enum class ECorrbolgAction : uint8
{
	RetrieveItem,
	StoreItem,
	LoadData,
	SaveData,
	Log
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
	UActorComponent* Owner = nullptr;

	/** The inventory to execute an action on. */
	TArray<FString>* StoredItems = nullptr;

	/** Additional data specific to the action, will be of a various types. */
	FInstancedStruct Payload = FInstancedStruct();

	/** Callback to execute when the action is finished. */
	TFunction<void(ECorrbolgActionResult)> Callback;
};

/**
* Mapping context to execute an action.
*/
USTRUCT(BlueprintType)
struct FCorrbolgActionMapping
{
	GENERATED_BODY()

public:
	/** The class of the action to execute. */
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCorrbolgAction> ActionClass = TSoftClassPtr<UCorrbolgAction>();

	/** Execute the action mapping to this mapping, if no instance exist ti will be created. */
	void ExecuteAction(const FCorrbolgActionContext& Context);

protected:
	/** Instance of the action mapped to this context. */
	UPROPERTY(Transient)
	TObjectPtr<UCorrbolgAction> ActionInstance = nullptr;
};
