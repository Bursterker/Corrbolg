// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Definitions/CorrbolgItemFragment.h"

#include "CorrbolgInventoryFragment.generated.h"

/**
 * Defines data to be used by inventories.
 */
UCLASS()
class CORRBOLG_API UCorrbolgInventoryFragment : public UCorrbolgItemFragment
{
	GENERATED_BODY()
	
public:
	const bool GetIsStackable() const { return bIsStackable; };
	const int GetRecommendedMaxStackSize() const { return RecommendedMaxStackSize; };

protected:
	/**
	* Wether this can be stacked in the inventory.
	*/
	UPROPERTY(EditDefaultsOnly)
	bool bIsStackable = false;

	/**
	* The recommended max stacksize, the inventory makes the final decision if this is used or not.
	* A value of 0 is interpretted as no reccomended size.
	*/
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bIsStackable", ClampMin = "0"))
	int RecommendedMaxStackSize = 0;
};
