#include "Actions/CorrbolgPrintDataToLog.h"

void UCorrbolgPrintDataToLog::Execute_Server_Implementation(const FActionContext& ActionContext)
{
	Super::Execute_Server_Implementation(ActionContext);

	FString StoredItemsString = "";

	const TArray<FString> StoredItemsCopy = *Context.StoredItems;
	for (const FString& Item : StoredItemsCopy)
	{
		StoredItemsString.Append(Item);
		StoredItemsString.Append(", ");
	}

	UE_LOG(LogTemp, Log, TEXT("Stored Items : %s"), *StoredItemsString);
}