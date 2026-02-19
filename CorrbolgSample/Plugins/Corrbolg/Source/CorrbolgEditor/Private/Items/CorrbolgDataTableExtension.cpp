// Copyright Epic Games, Inc. All Rights Reserved.

#include "Items/CorrbolgDataTableExtension.h"

#include "DataTableEditorUtils.h"
#include "Editor.h"
#include "Engine/DataTable.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "IDataTableEditor.h"
#include "Styling/AppStyle.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Textures/SlateIcon.h"

#include "Items/CorrbolgEditorItemSettings.h"
#include "Items/CorrbolgAutoFillRowIdInterface.h"

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

	const ICorrbolgAutoFillRowIdInterface* const FillRule = FindAutoFillRuleForDataTable(DataTable);
	if(!FillRule)
	{
		return;
	}

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
		UE_LOG(LogTemp, Log, TEXT("Fill Row IDs clicked for DataTable: %s"), *Table->GetPathName());

		const TMap<FName, uint8*>& RowMap = Table->GetRowMap();
		for (auto& RowEntry : RowMap)
		{
			const FTableRowBase* const EntryValue = (const FTableRowBase*)RowEntry.Value;
			if(!EntryValue)
			{
				UE_LOG(LogTemp, Warning, TEXT("Fill Row IDs: Row %s has null value"), *RowEntry.Key.ToString());
				continue;
			}

			const ICorrbolgAutoFillRowIdInterface* const FillRule = FindAutoFillRuleForDataTable(Table);
			const FName NewName = FillRule->GetRowId(EntryValue);

			if (NewName.IsNone())
			{
				UE_LOG(LogTemp, Warning, TEXT("Fill Row IDs: Fill rule returned None for row %s, skipping"), *RowEntry.Key.ToString());
				continue;
			}

			const bool bIsRenamed = FDataTableEditorUtils::RenameRow(Table, RowEntry.Key, NewName);

			if (!bIsRenamed)
			{
				UE_LOG(LogTemp, Warning, TEXT("Fill Row IDs: Failed to rename row %s to %s"), *RowEntry.Key.ToString(), *NewName.ToString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fill Row IDs: DataTable is no longer valid"));
	}
}

const ICorrbolgAutoFillRowIdInterface* const FCorrbolgDataTableExtension::FindAutoFillRuleForDataTable(const UDataTable* const DataTable)
{
	const UScriptStruct* const StructType = DataTable->GetRowStruct();

	const UCorrbolgEditorItemSettings* const EditorSettings = GetDefault<UCorrbolgEditorItemSettings>();
	if (!IsValid(EditorSettings))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get UCorrbolgEditorItemSettings"));
		return nullptr;
	}

	const TSubclassOf<UObject>* RuleClass = nullptr;
	for (const FAutoFillRowIdRuleDefinition& RuleDefinition : EditorSettings->AutoFillRowIdRuleDefinitions)
	{
		if (StructType->IsChildOf(RuleDefinition.RowType.LoadSynchronous()))
		{
			RuleClass = &RuleDefinition.Rule;
			break;
		}
	}

	if (!RuleClass)
	{
		return nullptr; // Auto Fill row is not defined (User Error or not needed for this table)
	}

	const ICorrbolgAutoFillRowIdInterface* const Interface = Cast<ICorrbolgAutoFillRowIdInterface>(RuleClass->GetDefaultObject());

	return Interface;
}

#undef LOCTEXT_NAMESPACE