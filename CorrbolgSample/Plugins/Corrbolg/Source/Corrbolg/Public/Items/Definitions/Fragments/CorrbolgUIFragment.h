// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Items/Definitions/CorrbolgItemFragment.h"

#include "CorrbolgUIFragment.generated.h"

/**
* Defines data on how to represent in the User Interface.
*/
UCLASS()
class CORRBOLG_API UCorrbolgUIFragment : public UCorrbolgItemFragment
{
	GENERATED_BODY()
	
public:
	const FText* FindText(const FGameplayTag& Tag) const;
	const TSoftObjectPtr<UTexture2D>& GetIcon() const { return Icon; };

protected:
	/**
	* Map to define text like name, description, flavor, ...
	*/
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "UI"))
	TMap<FGameplayTag, FText> TextMap = TMap<FGameplayTag, FText>();

	/**
	* Icon to represent the object.
	*/
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;
};
