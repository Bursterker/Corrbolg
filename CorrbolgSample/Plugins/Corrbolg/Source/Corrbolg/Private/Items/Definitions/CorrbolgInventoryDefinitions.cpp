// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Definitions/CorrbolgInventoryDefinitions.h"

FCorrbolgInventoryEntry::FCorrbolgInventoryEntry(const FGuid& Id, const FPrimaryAssetId& AssetId, const int Count)
{
	ObjectId = Id;
	PrimaryAssetId = AssetId;
	StackSize = Count;
}
