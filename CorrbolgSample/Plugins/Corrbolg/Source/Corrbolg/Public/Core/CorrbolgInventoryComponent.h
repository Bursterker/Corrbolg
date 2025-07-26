#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CorrbolgInventoryComponent.generated.h"

UENUM(BlueprintType)
enum class ECorrbolgAction : uint8
{
	StoreItem,
	LoadData,
	SaveData,
	Log
};

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
	/** Registers properties for replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Executed at the start of the lifetime of the component after initialization has finished. */
	virtual void BeginPlay() override;

	/** Checks if this lives on a server or authorative client. */
	virtual bool IsAuthorative() const;

#pragma region Manipulation
public:
	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual void ExecuteAction_Client(const ECorrbolgAction& Action);


	UFUNCTION(Server, Reliable)
	virtual void ExecuteAction_Server(const ECorrbolgAction& Action);
	virtual void ExecuteAction_Server_Implementation(const ECorrbolgAction& Action);

protected:
	/** Container holding all items stored in the inventory.*/
	UPROPERTY(Replicated)
	TArray<FString> StoredItems = TArray<FString>();

#pragma endregion

#pragma region Actions
protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<ECorrbolgAction, TSoftClassPtr<UCorrbolgAction>> ActionMapping = TMap<ECorrbolgAction, TSoftClassPtr<UCorrbolgAction>>();
#pragma endregion
};
