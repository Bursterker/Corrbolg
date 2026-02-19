// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class IAssetEditorInstance;
class IDataTableEditor;
class UDataTable;

/**
* Handles extension of DataTable editors with custom toolbar buttons and functionality.
* Listens for DataTable editors being opened and adds functionality.
*/
class FCorrbolgDataTableExtension
{
public:
	FCorrbolgDataTableExtension();
	~FCorrbolgDataTableExtension();

	/** Starts listening to asset editor open/close events. */
	void Initialize();

	/** Stops listening to asset editor events. */
	void Shutdown();

private:
	/** Called when an asset editor is opened. */
	void OnAssetEditorOpened(UObject* const Asset, IAssetEditorInstance* const EditorInstance) const;

	/** Adds the toolbar extension to a DataTable editor. */
	void ExtendDataTableEditorToolbar(UDataTable* const DataTable, IDataTableEditor* const DataTableEditor) const;

	/** Creates an extender for the toolbar and adds additional functionality to it. */
	TSharedRef<FExtender> CreateToolbarExtender(UDataTable* const DataTable, const IDataTableEditor* const DataTableEditor) const;

	/** Fills in the Row Id's with the assigned asset item ID. */
	static void FillDataTableRowIds(TWeakObjectPtr<UDataTable> DataTableWeak);

	static const ICorrbolgAutoFillRowIdInterface* const FindAutoFillRuleForDataTable(const UDataTable* const DataTable);

	/** Handle to the OnAssetOpenedInEditor delegate */
	FDelegateHandle AssetOpenedDelegateHandle;
};