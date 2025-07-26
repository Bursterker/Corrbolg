#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CorrbolgAction.generated.h"

/*
* Context for CorrbolgActions to provide information on what to execute an action on.
*/
USTRUCT(BlueprintType)
struct FActionContext
{
	GENERATED_BODY()

public:
	/** The inventory to execute an action on. */
	TArray<FString>* StoredItems = nullptr;

	/** 
	* Optional item to handle.
	* TODO: To be replaced with dynamic Data per action (example Save and log don't need an item).
	*/
	FString Item = "";
};

/**
* Base Interface to perform an action on the CorrbolgInventory.
*/
UCLASS()
class CORRBOLG_API UCorrbolgAction : public UObject
{
	GENERATED_BODY()
	
public:
	/** Executes the defined behavior. */
	UFUNCTION(Server, Reliable, Category = "Action")
	virtual void Execute_Server(const FActionContext& ActionContext);
	virtual void Execute_Server_Implementation(const FActionContext& ActionContext);

protected:
	/** The context the action was called for. */
	FActionContext Context = FActionContext();
};
