// Fill out your copyright notice in the Description page of Project Settings.


#include "Definitions/CorrbolgItemDefinition.h"

void UCorrbolgItemDefinition::PostInitProperties()
{
	Super::PostInitProperties();

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	// If the GUID is missing, generate one.
	if (!ItemId.IsValid())
	{ 
		ItemId = FGuid::NewGuid();
		
#if !WITH_EDITOR 
	// Log a warning in packaged to know something is wrong.
	UE_LOG(LogTemp, Warning, TEXT("ItemDefinition '%s' loaded with INVALID GUID in packaged build. A new GUID '%s' was generated at runtime, which may cause mismatches."), *GetName(), *ItemId.ToString());
#endif
	}
}

#if WITH_EDITOR 
void UCorrbolgItemDefinition::PostDuplicate(bool bDuplicateForPIE)
{
	Super::PostDuplicate(bDuplicateForPIE);

	if (!bDuplicateForPIE)
	{
		// Generate a new GUID for the duplicated asset.
		ItemId = FGuid::NewGuid();
		UE_LOG(LogTemp, Log, TEXT("ItemDefinition '%s' duplicated. New GUID '%s' generated."), *GetName(), *ItemId.ToString());
	}
}

bool UCorrbolgItemDefinition::CanEditChange(const FProperty* InProperty) const
{
	// Prevent scripts from editing the GUID.
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UCorrbolgItemDefinition, ItemId))
	{ 
		return false;
	}
	
	return Super::CanEditChange(InProperty);
}
#endif

template <typename T>
const T* UCorrbolgItemDefinition::FindFragmentOfClass() const
{
	for (const TSoftClassPtr<UCorrbolgItemFragment>& SoftFragment : Fragments)
	{
		const UClass* const Fragment = SoftFragment.LoadSynchronous();
		
		if (Fragment && Fragment->IsChildOf(T::StaticClass()))
		{
			return Cast<T>(Fragment->GetDefaultObject());
		}
	}

	return nullptr;
}
