// Copyright Epic Games, Inc. All Rights Reserved.

#include "CorrbolgDataTableAssetTypeActions.h"

#include "DataTableEditorModule.h"
#include "Engine/DataTable.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "IDataTableEditor.h"
#include "Modules/ModuleManager.h"
#include "Styling/AppStyle.h"
#include "Textures/SlateIcon.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

//////////////////////////////////////////////////////////////////////////
// FCorrbolgAssetTypeActions_DataTable

FCorrbolgAssetTypeActions_DataTable::FCorrbolgAssetTypeActions_DataTable(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FCorrbolgAssetTypeActions_DataTable::GetName() const
{
	return LOCTEXT("FCorrbolgDataTableAssetTypeActionsName", "Corrbolg Data Table");
}

FColor FCorrbolgAssetTypeActions_DataTable::GetTypeColor() const
{
	return FColor(192, 128, 128);
}

UClass* FCorrbolgAssetTypeActions_DataTable::GetSupportedClass() const
{
	return UDataTable::StaticClass();
}

void FCorrbolgAssetTypeActions_DataTable::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	for (UObject* Object : InObjects)
	{
		UDataTable* DataTable = Cast<UDataTable>(Object);
		if (DataTable != nullptr)
		{
			TSharedRef<IDataTableEditor> Editor = OpenDefaultEditor(DataTable, EditWithinLevelEditor);

			// Create the extenions for the toolbar.
			TSharedRef<FExtender> ToolbarExtender = CreateToolbarExtensions(Editor, DataTable);

			// Register the extender on this specific editor (no global duplicate editor appears)
			Editor->AddToolbarExtender(ToolbarExtender);

			// Ensure the toolbar is rebuilt so the button appears immediately
			Editor->RegenerateMenusAndToolbars();
		}
	}
}

uint32 FCorrbolgAssetTypeActions_DataTable::GetCategories()
{
	return MyAssetCategory;
}

TSharedRef<IDataTableEditor> FCorrbolgAssetTypeActions_DataTable::OpenDefaultEditor(UDataTable* const DataTable, TSharedPtr<class IToolkitHost> EditWithinLevelEditor) const
{
	// Use the engine DataTable editor and add a per-editor toolbar extender that captures the edited DataTable.
	FDataTableEditorModule& DataTableEditorModule = FModuleManager::LoadModuleChecked<FDataTableEditorModule>("DataTableEditor");

	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	// Create the engine DataTable editor (the real editor UI)
	TSharedRef<IDataTableEditor> Editor = DataTableEditorModule.CreateDataTableEditor(Mode, EditWithinLevelEditor, DataTable);

	return Editor;
}

TSharedRef<FExtender> FCorrbolgAssetTypeActions_DataTable::CreateToolbarExtensions(TSharedPtr<IDataTableEditor> Editor, UDataTable* const DataTable) const
{
	// Create an extender bound to this editor's command list so it's per-editor and context-aware.
	TSharedRef<FExtender> ToolbarExtender = MakeShareable(new FExtender());

	// Capture the DataTable with a weak ptr so the button callback can access it without pinning ownership.
	TWeakObjectPtr<UDataTable> DataTableWeak = DataTable;

	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		Editor->GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateLambda([DataTableWeak](FToolBarBuilder& ToolbarBuilder)
			{
				ToolbarBuilder.BeginSection("CorrbolgCommands");
				{
					ToolbarBuilder.AddToolBarButton(
						FUIAction(FExecuteAction::CreateLambda([DataTableWeak](){FCorrbolgAssetTypeActions_DataTable::Execute_FillRowIds(DataTableWeak);})),
						NAME_None,
						LOCTEXT("FillText", "Fill Row IDs"),
						LOCTEXT("FillTooltip", "Automatically fill ID cells in this DataTable"),
						FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Details")
					);
				}
				ToolbarBuilder.EndSection();
			})
	);

	return ToolbarExtender;
}

void FCorrbolgAssetTypeActions_DataTable::Execute_FillRowIds(TWeakObjectPtr<UDataTable> DataTableWeak)
{
	if (UDataTable* const Table = DataTableWeak.Get())
	{
		UE_LOG(LogTemp, Warning, TEXT("Fill Row IDs clicked for DataTable: %s"), *Table->GetPathName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fill Row IDs: DataTable is no longer valid"));
	}
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
