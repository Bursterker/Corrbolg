#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Items/Definitions/CorrbolgItemDefinition.h"

#include "CorrbolgActionContextFragments.generated.h"

// TODO: Koen: Creating a separate context is cumbersome, have the context be a variable table instead using gameplay tags as identifiers.
/** Base type for context fragments. Used to filter available structs in editor dropdowns. */
USTRUCT(BlueprintType)
struct FCorrbolgActionContextFragment
{
	GENERATED_BODY()

};

/**
* Contains context data for storage related actions: Store/Retrieve.
*/
USTRUCT(BlueprintType)
struct FCorrbolgStorageContextFragment : public FCorrbolgActionContextFragment
{
	GENERATED_BODY()

public:
	// TODO: Koen: The client shouldn't provide an item asset, only an item ID. The server is then responsible for fetching the data of the specified item. This cannot be part of the Client Payload.
	/** The item to handle with the action. */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCorrbolgItemDefinition> Item = nullptr;

	// TODO: Koen: The client cannot provide a stacksize, the server can only get this information from itself. This cannot be part of the Client Payload.
	/** The amount to store/retrieve. */
	UPROPERTY(BlueprintReadWrite)
	int StackSize = 0;
};

/**
* Contains context data for savegame related actions: Save/Load.
*/
USTRUCT(BlueprintType)
struct FCorrbolgSaveGameContextFragment : public FCorrbolgActionContextFragment
{
	GENERATED_BODY()

public:
	// TODO: Koen: The ItemTable should not be passed from the client, the client cannot delegate which table the server should use. The server needs to interpret it on its own. This cannot be part of the Client Payload.
	/** The database containing item definitions. */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UDataTable> ItemTable = nullptr;
};

/**
* Contains context data for log related actions..
*/
USTRUCT(BlueprintType)
struct FCorrbolgLogContextFragment : public FCorrbolgActionContextFragment
{
	GENERATED_BODY()

public:
	/** The gameplay tag to identify the name tag in the ui fragment. */
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag UINameTag = FGameplayTag::EmptyTag;
};