// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags.h"

#include "Actions/CorrbolgActionTypes.h"
#include "Actions/CorrbolgAction.h"

#include "CorrbolgInventorySettings.generated.h"

/**
 * 
 */
UCLASS()
class CORRBOLG_API UCorrbolgInventorySettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	* Actions mapped to execute on the inventory.
	* Key: Action identifier used to find wich action to execute.
	* Value: The class of the action to execute.
	*/
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TSoftClassPtr<UCorrbolgAction>> ActionMap = TMap<FGameplayTag, TSoftClassPtr<UCorrbolgAction>>();

};
