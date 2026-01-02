#pragma once

#include "Modules/ModuleInterface.h"

#include "Containers/Array.h"
#include "Containers/UnrealString.h"

#include "Templates/SharedPointer.h"

#include "AssetRegistry/AssetData.h"

#include "Dom/JsonValue.h"

class FMetadataExporterEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterMenus();
	void ExportAssetsToJson();
	void ResetExport();
	void GetAllAssets();
	template<typename TSerializer>
	void ExportMetadata(
		const TArray<FAssetData>& Assets,
		TArray<TSharedPtr<FJsonValue>>& OutExportedMetadata
	);
	void SaveToJson();

private:
	TArray<TSharedPtr<FJsonValue>> ExportedMetadata;
	TArray<FAssetData> Assets;
	FString OutputString;
};
