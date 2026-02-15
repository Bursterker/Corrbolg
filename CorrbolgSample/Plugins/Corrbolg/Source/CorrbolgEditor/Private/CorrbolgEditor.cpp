// Copyright Epic Games, Inc. All Rights Reserved.

#include "CorrbolgEditor.h"

#include "CorrbolgDataTableExtension.h"

#define LOCTEXT_NAMESPACE "FCorrbolgEditorModule"

void FCorrbolgEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	DataTableExtension = MakeShareable(new FCorrbolgDataTableExtension());
	DataTableExtension->Initialize();
}

void FCorrbolgEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	if (DataTableExtension.IsValid())
	{
		DataTableExtension->Shutdown();
		DataTableExtension.Reset();
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCorrbolgEditorModule, CorrbolgEditor)