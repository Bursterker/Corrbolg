// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CorrbolgAutoFillRowIdInterface.h"

#include "CorrbolgAutoFillRowNameRule.generated.h"

/**
*
*/
UCLASS(Blueprintable, BlueprintType)
class CORRBOLGEDITOR_API UCorrbolgAutoFillRowNameRule : public UObject, public ICorrbolgAutoFillRowIdInterface
{
	GENERATED_BODY()

public:
	virtual FName GetRowId(const FTableRowBase* const Row) const override;
};
