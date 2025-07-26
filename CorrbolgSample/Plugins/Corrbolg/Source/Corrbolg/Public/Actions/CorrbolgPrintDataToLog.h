#pragma once

#include "CoreMinimal.h"
#include "Actions/CorrbolgAction.h"

#include "CorrbolgPrintDataToLog.generated.h"

/**
* Action to print all items stored in the inventory to the log.
*/
UCLASS()
class CORRBOLG_API UCorrbolgPrintDataToLog : public UCorrbolgAction
{
	GENERATED_BODY()
	
public:
	virtual void Execute_Server_Implementation(const FActionContext& ActionContext) override;

};
