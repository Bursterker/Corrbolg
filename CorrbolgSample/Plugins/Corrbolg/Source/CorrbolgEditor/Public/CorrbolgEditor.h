// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "AssetTypeActions_Base.h"

class FCorrbolgEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Registers asset type actions with the asset tools module */
	void RegisterAssetTypeActions();

	/** Unregisters asset type actions */
	void UnregisterAssetTypeActions();

	/** Array of registered asset type actions */
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
};
