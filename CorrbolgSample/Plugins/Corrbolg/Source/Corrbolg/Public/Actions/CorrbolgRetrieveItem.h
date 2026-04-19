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
	virtual void Client_PerformAction_Implementation() override;

	virtual void Server_PerformAction_Implementation() override;
};
