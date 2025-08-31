// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CorrbolgAction.h"

#include "CorrbolgRetrieveItem.generated.h"

/**
* Action to retrieve an item from the Corrbolg Inventory.
*/
UCLASS()
class CORRBOLG_API UCorrbolgRetrieveItem : public UCorrbolgAction
{
	GENERATED_BODY()
	
protected:
	virtual void PerformAction(const FCorrbolgActionContext& ActionContext) const override;

	/** Retrives an item from the inventory. */
	UFUNCTION(Server, Reliable, Category = "Action|Manipulation")
	virtual void RetrieveItem_Server() const;
	virtual void RetrieveItem_Server_Implementation() const;
};
