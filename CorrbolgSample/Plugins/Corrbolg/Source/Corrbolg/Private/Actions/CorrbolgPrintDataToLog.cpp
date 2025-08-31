#include "Actions/CorrbolgPrintDataToLog.h"

void UCorrbolgPrintDataToLog::PerformAction(const FCorrbolgActionContext& ActionContext) const
{
	FString StoredItemsString = "";

	const TArray<FString> StoredItemsCopy = *Context.StoredItems;
	for (const FString& Item : StoredItemsCopy)
	{
		StoredItemsString.Append(Item);
		StoredItemsString.Append(", ");
	}

	UE_LOG(LogTemp, Log, TEXT("Stored Items : %s"), *StoredItemsString);
	
	OnActionFinished.Broadcast(ECorrbolgActionResult::Success);
}
