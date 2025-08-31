#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructUtils/InstancedStruct.h"

#include "CorrbolgInventoryComponent.generated.h"

enum class ECorrbolgActionResult : uint8;

struct FCorrbolgActionContext;
struct FCorrbolgActionMapping;

class UCorrbolgAction;

/**
* Main entry point to manage an inventory.
*/
UCLASS(Blueprintable, ClassGroup=(Corrbolg), meta=(BlueprintSpawnableComponent) )
class CORRBOLG_API UCorrbolgInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCorrbolgInventoryComponent();

protected:
	/** Container holding all items stored in the inventory.*/
	UPROPERTY(Replicated)
	TArray<FString> StoredItems = TArray<FString>();

	/** Registers properties for replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Executed at the start of the lifetime of the component after initialization has finished. */
	virtual void BeginPlay() override;

	/** Checks if this lives on a server or authorative client. */
	virtual bool IsAuthorative() const;

#pragma region Actions
public:
	/** Asks the server to perform an action on the inventory. */
	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual void ExecuteAction_Client(const ECorrbolgAction& Action, const FInstancedStruct& Payload = FInstancedStruct());

	/** Calls the relevant action to execute on the inventory. */
	UFUNCTION(Server, Reliable)
	virtual void ExecuteAction_Server(const ECorrbolgAction& Action, const FInstancedStruct& Payload = FInstancedStruct());
	virtual void ExecuteAction_Server_Implementation(const ECorrbolgAction& Action, const FInstancedStruct& Payload = FInstancedStruct());

protected:
	/** Available actions mapped to execute on the inventory. */
	UPROPERTY(EditDefaultsOnly)
	TMap<ECorrbolgAction, FCorrbolgActionMapping> ActionMap = TMap<ECorrbolgAction, FCorrbolgActionMapping>();

	/** Callback from when any action has finished. */
	UFUNCTION()
	virtual void OnActionExecutionFinished(const ECorrbolgActionResult Result);

	bool bIsHandlingAction = false;
#pragma endregion
};
