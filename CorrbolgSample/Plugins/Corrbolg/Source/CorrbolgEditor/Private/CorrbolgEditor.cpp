// Copyright Epic Games, Inc. All Rights Reserved.

#include "CorrbolgEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"

#include "CorrbolgDataTableAssetTypeActions.h"

#define LOCTEXT_NAMESPACE "FCorrbolgEditorModule"

void FCorrbolgEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterAssetTypeActions();
}

void FCorrbolgEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UnregisterAssetTypeActions();
}

void FCorrbolgEditorModule::RegisterAssetTypeActions()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// Create and register our DataTable asset type actions
	TSharedRef<FCorrbolgAssetTypeActions_DataTable> DataTableActions =
		MakeShareable(new FCorrbolgAssetTypeActions_DataTable(
			AssetTools.RegisterAdvancedAssetCategory(FName("Corrbolg"), LOCTEXT("CorrbolgCategory", "Corrbolg"))
		));

	RegisteredAssetTypeActions.Add(DataTableActions);
	AssetTools.RegisterAssetTypeActions(DataTableActions);
}

void FCorrbolgEditorModule::UnregisterAssetTypeActions()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		for (TSharedRef<IAssetTypeActions> RegisteredAction : RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(RegisteredAction);
		}
	}

	RegisteredAssetTypeActions.Empty();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCorrbolgEditorModule, CorrbolgEditor)