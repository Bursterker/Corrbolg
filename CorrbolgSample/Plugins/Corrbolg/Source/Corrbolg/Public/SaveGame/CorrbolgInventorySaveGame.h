#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "CorrbolgInventorySaveGame.generated.h"

namespace CorrbolgSaveGame
{
	/** Name of the save slot to use when saving data. */
	const FString SaveSlotName = "CorrbolgSaveSlot";

	/** User index to use when saving data.*/
	const int SaveUserIndex = 0;
}

/**
* SaveGame data for the inventory.
* Keep in sync with FCorrbolgInventorySaveGameData.
*/
UCLASS(ClassGroup = (Corrbolg))
class CORRBOLG_API UCorrbolgInventorySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Sets default values for this SaveGame's properties
	UCorrbolgInventorySaveGame();

	/** Items saved in the inventory. */
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TArray<FString> SavedStoredItems = TArray<FString>();
};

/**
* Save game data to send over the network.
* Keep in sync with UCorrbolgInventorySaveGame.
*/
USTRUCT(BlueprintType)
struct FCorrbolgInventorySaveGameData
{
	GENERATED_BODY()

	FCorrbolgInventorySaveGameData();

	/** Items saved in the inventory. */
	UPROPERTY()
	TArray<FString> SavedStoredItems = TArray<FString>();
};
