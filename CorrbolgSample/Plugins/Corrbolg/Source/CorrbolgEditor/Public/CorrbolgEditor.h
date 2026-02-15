// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FCorrbolgDataTableExtension;

class FCorrbolgEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** DataTable editor extension handler */
	TSharedPtr<FCorrbolgDataTableExtension> DataTableExtension;
};
