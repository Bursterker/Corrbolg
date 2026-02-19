// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "CorrbolgItemDefinition.generated.h"

class UCorrbolgItemFragment;

/**
* Defines the make up of an item.
*/
UCLASS(BlueprintType, Blueprintable)
class CORRBOLG_API UCorrbolgItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;

#if WITH_EDITOR
	virtual void PostDuplicate(bool bDuplicateForPIE) override;
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif

	const FGuid& GetId() const { return ItemId; };
	
	template <typename T>
	const T* FindFragmentOfClass() const;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, AssetRegistrySearchable)
	FGuid ItemId = FGuid();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftClassPtr<UCorrbolgItemFragment>> Fragments = TArray<TSoftClassPtr<UCorrbolgItemFragment>>();
};
