#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"

#include "Inventory/Definitions/CorrbolgInventoryDefinitions.h"

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
	/** Settings to apply to this inventory. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCorrbolgInventorySettings> InventorySettings = nullptr;

	/** Executed at the start of the lifetime of the component after initialization has finished. */
	virtual void BeginPlay() override;

#pragma region Core
public:
	const TArray<FCorrbolgInventoryEntry>& GetStoredEntries() const { return StoredEntries; };
	TArray<FCorrbolgInventoryEntry>& GetStoredEntries() { return StoredEntries; };

private:
	/** Container holding all items stored in the inventory. */
	UPROPERTY(Replicated)
	TArray<FCorrbolgInventoryEntry> StoredEntries = TArray<FCorrbolgInventoryEntry>();

#pragma endregion

#pragma region Replication
protected:
	/** Registers properties for replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
#pragma endregion

#pragma region Actions
public:
	/** Request execution of an action can be called from Client and Server. */
	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual void RequestAction(const FGameplayTag& ActionId, const FInstancedStruct& Payload = FInstancedStruct());

	/** Processes the action queue, handling actions in order. */
	void ProcessActionQueue();

protected:
	/** Retrieves the action template class for a given action ID. */
	virtual TSoftClassPtr<UCorrbolgAction>* GetActionTemplate(const FGameplayTag& ActionId);

	/** Queue holding all pending actions to be processed. */
	TQueue<TObjectPtr<UCorrbolgAction>> ActionQueue;
#pragma endregion
};
