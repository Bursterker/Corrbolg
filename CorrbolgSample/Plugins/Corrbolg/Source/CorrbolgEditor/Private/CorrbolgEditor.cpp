// Copyright Epic Games, Inc. All Rights Reserved.

#include "CorrbolgEditor.h"

#pragma region Extender includes
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Commands/UIAction.h"
#include "Modules/ModuleManager.h"
#include "DataTableEditorModule.h"
#include <Toolkits/AssetEditorToolkit.h>
#include <Framework/MultiBox/MultiBoxExtender.h>
#include <Templates/SharedPointer.h>
#include <Textures/SlateIcon.h>
#include <Styling/AppStyle.h>
#pragma endregion

#define LOCTEXT_NAMESPACE "FCorrbolgEditorModule"

void FCorrbolgEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	ExtendDataTableRowEditor();
}

void FCorrbolgEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FCorrbolgEditorModule::ExtendDataTableRowEditor() const
{
	FDataTableEditorModule& DataTableEditorModule = FModuleManager::LoadModuleChecked<FDataTableEditorModule>("DataTableEditor");

	TSharedPtr<FExtensibilityManager> ToolbarExtender = DataTableEditorModule.GetToolBarExtensibilityManager();
	ToolbarExtender->AddExtender(CreateDataTableToolbarExtender());
}

TSharedRef<FExtender> FCorrbolgEditorModule::CreateDataTableToolbarExtender() const
{
	TSharedRef<FExtender> Extender = MakeShared<FExtender>();

	Extender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		nullptr,
		FToolBarExtensionDelegate::CreateRaw(this, &FCorrbolgEditorModule::FillToolbar)
	);

	return Extender;
}

void FCorrbolgEditorModule::FillToolbar(FToolBarBuilder& ToolbarBuilder) const
{
	ToolbarBuilder.BeginSection("CorrbolgCommands");
	{
		ToolbarBuilder.AddToolBarButton(
			FUIAction(FExecuteAction::CreateRaw(this, &FCorrbolgEditorModule::OnFillRowIdClicked)),
			NAME_None,
			LOCTEXT("FillText", "Fill"),
			LOCTEXT("FillTooltip", "Fill this DataTable Ids"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Details"));

	}
	ToolbarBuilder.EndSection();
}

void FCorrbolgEditorModule::OnFillRowIdClicked() const
{
	UE_LOG(LogTemp, Warning, TEXT("OnFillRowIdClicked toolbar button clicked"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCorrbolgEditorModule, CorrbolgEditor)