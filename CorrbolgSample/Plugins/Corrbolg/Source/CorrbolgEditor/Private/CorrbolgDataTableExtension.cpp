// Copyright Epic Games, Inc. All Rights Reserved.

#include "CorrbolgDataTableExtension.h"

#include "Editor.h"
#include "Engine/DataTable.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "IDataTableEditor.h"
#include "Styling/AppStyle.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Textures/SlateIcon.h"

#define LOCTEXT_NAMESPACE "FCorrbolgDataTableExtension"

FCorrbolgDataTableExtension::FCorrbolgDataTableExtension()
	: AssetOpenedDelegateHandle()
{
}

FCorrbolgDataTableExtension::~FCorrbolgDataTableExtension()
{
	Shutdown();
}

void FCorrbolgDataTableExtension::Initialize()
{
	if (!IsValid(GEditor))
	{
		return;
	}

	UAssetEditorSubsystem* const AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	if (!IsValid(AssetEditorSubsystem))
	{
		return;
	}

	// Listen for when DataTable editors are opened
	AssetOpenedDelegateHandle = AssetEditorSubsystem->OnAssetOpenedInEditor().AddRaw(this, &FCorrbolgDataTableExtension::OnAssetEditorOpened);
}

void FCorrbolgDataTableExtension::Shutdown()
{
	if (!IsValid(GEditor))
	{
		return;
	}

	UAssetEditorSubsystem* const AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	if (!IsValid(AssetEditorSubsystem))
	{
		return;
	}

	if (AssetOpenedDelegateHandle.IsValid())
	{
		AssetEditorSubsystem->OnAssetOpenedInEditor().Remove(AssetOpenedDelegateHandle);
		AssetOpenedDelegateHandle.Reset();
	}
}

void FCorrbolgDataTableExtension::OnAssetEditorOpened(UObject* const Asset, IAssetEditorInstance* const EditorInstance) const
{
	if (!IsValid(Asset) || !EditorInstance)
	{
		return;
	}

	UDataTable* const DataTable = Cast<UDataTable>(Asset);
	if (!IsValid(DataTable))
	{
		return;
	}

	// TODO: Koen: Only extend if the asset has a compatible row struct.

	IDataTableEditor* const DataTableEditor = static_cast<IDataTableEditor*>(EditorInstance);
	if (!DataTableEditor)
	{
		return;
	}

	ExtendDataTableEditorToolbar(DataTable, DataTableEditor);
}

void FCorrbolgDataTableExtension::ExtendDataTableEditorToolbar(UDataTable* const DataTable, IDataTableEditor* const DataTableEditor) const
{
	// Create an extender for this specific editor.
	TSharedRef<FExtender> ToolbarExtender = CreateToolbarExtender(DataTable, DataTableEditor);

	// Add the extender to this specific editor.
	DataTableEditor->AddToolbarExtender(ToolbarExtender);
	DataTableEditor->RegenerateMenusAndToolbars();
}

TSharedRef<FExtender> FCorrbolgDataTableExtension::CreateToolbarExtender(UDataTable* const DataTable, const IDataTableEditor* const DataTableEditor) const
{
	TWeakObjectPtr<UDataTable> DataTableWeak = DataTable;
	TSharedRef<FExtender> ToolbarExtender = MakeShareable(new FExtender());

	ToolbarExtender->AddToolBarExtension(
		"DataTableCommands", // FDataTableEditor::FillToolbar adds its buttons to the "DataTableCommands" section, so we want to add after that
		EExtensionHook::After,
		DataTableEditor->GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateLambda([DataTableWeak](FToolBarBuilder& ToolbarBuilder)
			{
				ToolbarBuilder.BeginSection("CorrbolgCommands");
				{
					ToolbarBuilder.AddToolBarButton(
						FUIAction(FExecuteAction::CreateLambda([DataTableWeak]() 
							{ FCorrbolgDataTableExtension::FillDataTableRowIds(DataTableWeak); })),
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

void FCorrbolgDataTableExtension::FillDataTableRowIds(TWeakObjectPtr<UDataTable> DataTableWeak)
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

#undef LOCTEXT_NAMESPACE