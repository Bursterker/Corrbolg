#pragma once
#include "CoreMinimal.h"

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
	FString Item = "";
};
