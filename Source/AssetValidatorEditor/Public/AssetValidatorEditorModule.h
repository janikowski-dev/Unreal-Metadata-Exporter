#pragma once

#include "Modules/ModuleInterface.h"

class FAssetValidatorEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterMenus();
	void ExportAssetsToJson();
	void ResetExport();
	void GetAllAssets();
	void ExportMeshes();
	void ExportTextures();
	void SaveToJson();

private:
	TArray<TSharedPtr<FJsonValue>> ExportedAssets;
	TArray<FAssetData> Assets;
	FString OutputString;
};
