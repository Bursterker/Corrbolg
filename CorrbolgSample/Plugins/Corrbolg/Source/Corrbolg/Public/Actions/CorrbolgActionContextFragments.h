#pragma once
#include "CoreMinimal.h"

#include "Items/Definitions/CorrbolgItemDefinition.h"

#include "CorrbolgActionContextFragments.generated.h"

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
	/** The item to handle with the action. */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCorrbolgItemDefinition> Item = nullptr;

	/** The amount to store/retrieve*/
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
	/** The database containing item definitions. */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UDataTable> ItemTable = nullptr;
};