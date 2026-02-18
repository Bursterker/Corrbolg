// Fill out your copyright notice in the Description page of Project Settings.

#include "CorrbolgAutoFillRowNameRule.h"

#include "Definitions/CorrbolgItemTableRow.h"

FName UCorrbolgAutoFillRowNameRule::GetRowId(const FTableRowBase* const Row) const
{
	const FCorrbolgItemTableRow* const ItemRow = (FCorrbolgItemTableRow*)(Row);
	if (!ensureAlwaysMsgf(ItemRow, TEXT("Trying to get the Row Id but, the row is not of type FCorrbolgItemTableRow.")))
	{
		return NAME_None;
	}

	const UCorrbolgItemDefinition* const ItemDefinition = ItemRow->ItemDefinition.LoadSynchronous();
	if (!IsValid(ItemDefinition))
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to get the Row Id but, there is no ItemDefinition."));
		return NAME_None;
	}

	const FName NewName = *ItemDefinition->GetId().ToString();

	return NewName;
}
