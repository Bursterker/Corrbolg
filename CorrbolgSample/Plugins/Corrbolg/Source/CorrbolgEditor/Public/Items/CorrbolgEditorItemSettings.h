// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/DataTable.h"

#include "CorrbolgEditorItemSettings.generated.h"

USTRUCT()
struct FAutoFillRowIdRuleDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowTreeView, HideViewOptions))
	TSoftObjectPtr<const UScriptStruct> RowType;

	UPROPERTY(EditAnywhere, meta = (ShowTreeView, HideViewOptions, MustImplement = "/Script/CorrbolgEditor.CorrbolgAutoFillRowIdInterface"))
	TSubclassOf<UObject> Rule;
};

/**
 * 
 */
UCLASS(config = Plugins, DefaultConfig)
class CORRBOLGEDITOR_API UCorrbolgEditorItemSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, config, Category = "CorrBolgEditor")
	TArray<FAutoFillRowIdRuleDefinition> AutoFillRowIdRuleDefinitions;

};
