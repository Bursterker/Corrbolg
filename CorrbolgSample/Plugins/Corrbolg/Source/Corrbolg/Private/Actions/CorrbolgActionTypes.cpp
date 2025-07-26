#include "Actions/CorrbolgActionTypes.h"

#include "Actions/CorrbolgAction.h"

void FCorrbolgActionMapping::ExecuteAction(const FActionContext& Context)
{
	if (!IsValid(ActionInstance))
	{
		if (!ensureMsgf(ActionClass.IsValid(), TEXT("Invalid action mapping class!")))
		{
			return;
		}

		ActionInstance = NewObject<UCorrbolgAction>(Context.Owner, ActionClass.LoadSynchronous());
	}

	ActionInstance->Execute_Server(Context);
}
