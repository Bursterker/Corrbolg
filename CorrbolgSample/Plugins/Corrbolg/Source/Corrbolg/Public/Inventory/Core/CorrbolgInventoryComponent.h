#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"

#include "Items/Definitions/CorrbolgInventoryDefinitions.h"

#include "CorrbolgInventoryComponent.generated.h"

enum class ECorrbolgActionResult : uint8;

class UCorrbolgAction;
class UCorrbolgInventorySettings;

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
	TArray<FCorrbolgInventoryEntry> StoredEntries = TArray<FCorrbolgInventoryEntry>();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCorrbolgInventorySettings> InventorySettings = nullptr;

	/** Executed at the start of the lifetime of the component after initialization has finished. */
	virtual void BeginPlay() override;

#pragma region Replication
protected:
	/** Registers properties for replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Checks if this lives on a server or authorative client. */
	virtual bool IsAuthorative() const;
#pragma endregion

#pragma region Actions
public:
	/** Asks the server to perform an action on the inventory. */
	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual void ExecuteAction_Client(const FGameplayTag& ActionId, const FInstancedStruct& Payload = FInstancedStruct());

	/** Calls the relevant action to execute on the inventory. */
	UFUNCTION(Server, Reliable)
	virtual void ExecuteAction_Server(const FGameplayTag& ActionId, const FInstancedStruct& Payload = FInstancedStruct());
	virtual void ExecuteAction_Server_Implementation(const FGameplayTag& ActionId, const FInstancedStruct& Payload = FInstancedStruct());

protected:
	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UCorrbolgAction>> ActionInstanceMap;

	UPROPERTY(Transient)
	TObjectPtr<UCorrbolgAction> ActiveAction = nullptr;

	/** Callback from when any action has finished. */
	UFUNCTION()
	virtual void OnActionExecutionFinished(const ECorrbolgActionResult Result);
	
	/** Initialize the actions from the settings. */
	UFUNCTION()
	virtual void InitializeActions();
#pragma endregion
};
