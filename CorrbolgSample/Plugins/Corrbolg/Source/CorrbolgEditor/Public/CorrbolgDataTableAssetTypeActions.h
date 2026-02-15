// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AssetTypeActions_Base.h"
#include <IDataTableEditor.h>

/**
* Implements an action for UDataTable assets.
*/
class FCorrbolgAssetTypeActions_DataTable : public FAssetTypeActions_Base
{
public:
	FCorrbolgAssetTypeActions_DataTable(EAssetTypeCategories::Type InAssetCategory);

	// IAssetTypeActions interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;
	// End of IAssetTypeActions interface

protected:
	virtual TSharedRef<IDataTableEditor> OpenDefaultEditor(UDataTable* const DataTable, TSharedPtr<class IToolkitHost> EditWithinLevelEditor) const;
	virtual TSharedRef<FExtender> CreateToolbarExtensions(TSharedPtr<IDataTableEditor> Editor, UDataTable* const DataTable) const;
	static void Execute_FillRowIds(TWeakObjectPtr<UDataTable> DataTableWeak);

private:
	EAssetTypeCategories::Type MyAssetCategory;
};
