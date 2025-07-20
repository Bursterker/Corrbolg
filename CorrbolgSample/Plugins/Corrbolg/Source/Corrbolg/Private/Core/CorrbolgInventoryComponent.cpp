#include "Core/CorrbolgInventoryComponent.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

#include "SaveGame/CorrbolgInventorySaveGame.h"

UCorrbolgInventoryComponent::UCorrbolgInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCorrbolgInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCorrbolgInventoryComponent, StoredItems);
}

void UCorrbolgInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsAuthorative())
	{
		LoadInventory_Server_Implementation();
	}
}

bool UCorrbolgInventoryComponent::IsAuthorative() const
{
	return IsValid(GetOwner()) && GetOwner()->HasAuthority();
}

#pragma region Manipulation
void UCorrbolgInventoryComponent::StoreItem_Client(const FString& Item)
{
	if (!ensureMsgf(!Item.IsEmpty(), TEXT("Trying to store an undefined item! This is not supported.")))
	{
		return;
	}

	StoreItem_Server(Item);
}

void UCorrbolgInventoryComponent::StoreItem_Server_Implementation(const FString& Item)
{
	// Verify the call is on the server/authorative client.
	if (!IsAuthorative())
	{
		return;
	}

	StoredItems.Add(Item);

	SaveInventory_Server();
}
#pragma endregion

#pragma region SaveGame
void UCorrbolgInventoryComponent::SaveInventory_Client_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData)
{
	UCorrbolgInventorySaveGame* const SaveGameInstance =
		Cast<UCorrbolgInventorySaveGame>(
			UGameplayStatics::CreateSaveGameObject(UCorrbolgInventorySaveGame::StaticClass()));

	SaveGameInstance->SavedStoredItems = SaveGameData.SavedStoredItems;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, SaveUserIndex);
}

void UCorrbolgInventoryComponent::SaveInventory_Server_Implementation()
{
	// Verify the call is on the server/authorative client.
	if (!IsAuthorative())
	{
		return;
	}

	FCorrbolgInventorySaveGameData SaveData = FCorrbolgInventorySaveGameData();
	SaveData.SavedStoredItems = StoredItems;

	SaveInventory_Client(SaveData);
}

void UCorrbolgInventoryComponent::LoadInventory_Client_Implementation()
{
	UCorrbolgInventorySaveGame* const SaveGameInstance =
		Cast<UCorrbolgInventorySaveGame>(
			UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));

	if (!IsValid(SaveGameInstance))
	{
		return;
	}

	FCorrbolgInventorySaveGameData SaveGameData = FCorrbolgInventorySaveGameData();
	SaveGameData.SavedStoredItems = SaveGameInstance->SavedStoredItems;

	OnSaveDataReceived_Server(SaveGameData);
}

void UCorrbolgInventoryComponent::LoadInventory_Server_Implementation()
{
	// Verify the call is on the server/authorative client.
	if (!IsAuthorative())
	{
		return;
	}

	LoadInventory_Client();
}

void UCorrbolgInventoryComponent::OnSaveDataReceived_Server_Implementation(const FCorrbolgInventorySaveGameData& SaveGameData)
{
	// Verify the call is on the server/authorative client.
	if (!IsAuthorative())
	{
		return;
	}

	StoredItems = SaveGameData.SavedStoredItems;
}
#pragma endregion
