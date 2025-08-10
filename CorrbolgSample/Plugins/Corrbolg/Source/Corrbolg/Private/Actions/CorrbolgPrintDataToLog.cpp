#include "Actions/CorrbolgPrintDataToLog.h"

ECorrbolgActionResult UCorrbolgPrintDataToLog::PerformAction_Server(const FCorrbolgActionContext& ActionContext) const
{
	FString StoredItemsString = "";

	const TArray<FString> StoredItemsCopy = *Context.StoredItems;
	for (const FString& Item : StoredItemsCopy)
	{
		StoredItemsString.Append(Item);
		StoredItemsString.Append(", ");
	}

	UE_LOG(LogTemp, Log, TEXT("Stored Items : %s"), *StoredItemsString);

	return ECorrbolgActionResult::Success;
}
