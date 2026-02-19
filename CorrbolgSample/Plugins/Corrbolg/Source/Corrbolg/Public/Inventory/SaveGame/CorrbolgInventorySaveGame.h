#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "Items/Definitions/CorrbolgInventoryDefinitions.h"

#include "CorrbolgInventorySaveGame.generated.h"

namespace CorrbolgSaveGame
{
	/** Name of the save slot to use when saving data. */
	const FString SaveSlotName = "CorrbolgSaveSlot";

	/** User index to use when saving data.*/
	const int SaveUserIndex = 0;
}

/**
* Save game data of an inventory entry.
*/
USTRUCT(BlueprintType)
struct FCorrbolgInventoryEntrySaveGameData
{
	GENERATED_BODY()

	/** The ID of the object represented in the entry to save. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid ObjectId = FGuid();

	/** Amount of instances stored in the entry to save. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	int32 StackSize = 0;
};

/**
* Save game data to send over the network.
*/
USTRUCT(BlueprintType)
struct FCorrbolgInventorySaveGameData
{
	GENERATED_BODY()

	FCorrbolgInventorySaveGameData();

	/** Entries saved in the inventory. */
	UPROPERTY()
	TArray<FCorrbolgInventoryEntrySaveGameData> SavedInventoryEntries = TArray<FCorrbolgInventoryEntrySaveGameData>();
};

/**
* SaveGame data for the inventory.
*/
UCLASS(ClassGroup = (Corrbolg))
class CORRBOLG_API UCorrbolgInventorySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Sets default values for this SaveGame's properties
	UCorrbolgInventorySaveGame();

	/** Data that is saved for the inventory. */
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FCorrbolgInventorySaveGameData SaveGameData = FCorrbolgInventorySaveGameData();
};

