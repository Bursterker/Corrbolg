#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CorrbolgInventoryComponent.generated.h"

class UCorrbolgInventorySaveGame;

struct FCorrbolgInventorySaveGameData;

/**
* Main entry point to manage an inventory.
*/
UCLASS( ClassGroup=(Corrbolg), meta=(BlueprintSpawnableComponent) )
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
	/** Request the server to store an item from the client. */
	UFUNCTION(BlueprintCallable, Category = "Core")
	virtual void StoreItem_Client(const FString& Item);

protected:
	/** Container holding all items stored in the inventory.*/
	UPROPERTY(Replicated)
	TArray<FString> StoredItems = TArray<FString>();

	/** Stores an item in the inventory. */
	UFUNCTION(Server, Reliable)
	virtual void StoreItem_Server(const FString& Item);
	virtual void StoreItem_Server_Implementation(const FString& Item);

#pragma endregion

#pragma region SaveGame
protected:
	/** Writes the inventory data to the save game. */
	UFUNCTION(Client, Reliable)
	virtual void SaveInventory_Client(const FCorrbolgInventorySaveGameData& SaveGameData);
	virtual void SaveInventory_Client_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData);
	
	/** Creates the save game object and sends it to the client for local save. */
	UFUNCTION(Server, Reliable)
	virtual void SaveInventory_Server();
	virtual void SaveInventory_Server_Implementation();
	
	/** Reads the inventory data from the save game and sends it to the server for replication. */
	UFUNCTION(Client, Reliable)
	virtual void LoadInventory_Client();
	virtual void LoadInventory_Client_Implementation();
	
	/** Request the client for the local save game data. */
	UFUNCTION(Server, Reliable)
	virtual void LoadInventory_Server();
	virtual void LoadInventory_Server_Implementation();

	/** Replicates the received local save game to all clients. */
	UFUNCTION(Server, Reliable)
	virtual void OnSaveDataReceived_Server(const FCorrbolgInventorySaveGameData& SaveGameData);
	virtual void OnSaveDataReceived_Server_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData);

private:
	/** Name of the save slot to use when saving data. */
	const FString SaveSlotName = "CorrbolgSaveSlot";

	/** User index to use when saving data.*/
	const int SaveUserIndex = 0;
#pragma endregion
};
