// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "Items/Definitions/CorrbolgItemDefinition.h"

#include "CorrbolgItemTableRow.generated.h"

/**
* Row base for ItemTables.
*/
USTRUCT(BlueprintType)
struct CORRBOLG_API FCorrbolgItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UCorrbolgItemDefinition> ItemDefinition = nullptr;
};
