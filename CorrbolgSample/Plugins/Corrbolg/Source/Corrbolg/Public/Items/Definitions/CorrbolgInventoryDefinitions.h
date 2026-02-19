// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CorrbolgInventoryDefinitions.generated.h"

USTRUCT(BlueprintType)
struct FCorrbolgInventoryEntry
{
	GENERATED_BODY()

public:
	FCorrbolgInventoryEntry() {};

	/**
	* Construct the entry definition.
	* @param Id: The Id of the object being stored.
	* @param AssetId: The Primary Asset Id of the asset that defines the object.
	* @param Count: Number of objects to store.
	*/
	FCorrbolgInventoryEntry(const FGuid& Id, const FPrimaryAssetId& AssetId, const int Count);

	const FGuid& GetObjectId() const { return ObjectId; };
	const FPrimaryAssetId& GetAssetId() const { return PrimaryAssetId; };
	const int32 GetStackSize() const { return StackSize; };

protected:
	/**
	* The ID of the object represented in the entry.
	* Used to find the correct asset id when loading from the savegame.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid ObjectId = FGuid();
	
	/**
	* The asset Id of the asset containing the represented object data.
	* Used to directly load data with the asset manager.
	* Set at runtime to ensure stability between updates. 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId();
	
	/**
	* Amount of instances stored in this entry.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	int32 StackSize = 0;
};
