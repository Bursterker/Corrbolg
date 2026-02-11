// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FCorrbolgEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	// TODO: Koen: Move this to another file as a static method, so that we don't have monolithic includes.
	void ExtendDataTableRowEditor() const;
	TSharedRef<FExtender> CreateDataTableToolbarExtender() const;
	void FillToolbar(FToolBarBuilder& ToolbarBuilder) const;
	void OnFillRowIdClicked() const;
};
