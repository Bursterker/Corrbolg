// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Definitions/Fragments/CorrbolgUIFragment.h"

const FText* UCorrbolgUIFragment::FindText(const FGameplayTag& Tag) const
{
	return TextMap.Find(Tag);
}
