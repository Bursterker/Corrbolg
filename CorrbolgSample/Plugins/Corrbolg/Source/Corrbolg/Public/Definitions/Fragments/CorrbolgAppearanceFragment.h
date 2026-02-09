// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Definitions/CorrbolgItemFragment.h"
#include "CorrbolgAppearanceFragment.generated.h"

/**
 * Defines data to be used for appearance in the world.
 */
UCLASS()
class CORRBOLG_API UCorrbolgAppearanceFragment : public UCorrbolgItemFragment
{
	GENERATED_BODY()

public:
	const TSoftObjectPtr<UObject>& GetMesh() const { return Mesh; };

protected:
	/**
	* The mesh to represent the object in the world.
	*/
	UPROPERTY(EditDefaultsOnly, meta = (AllowedClasses = "/Script/Engine.UStaticMesh,/Script/Engine.USkeletalMesh"))
	TSoftObjectPtr<UObject> Mesh;
};
