#include "MetadataExporterEditorModule.h"

#include "Exporter/MeshSerializer.h"
#include "Exporter/TextureSerializer.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/AssetData.h"

#include "ToolMenus.h"

#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"

#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

#include "Dom/JsonObject.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

IMPLEMENT_MODULE(FMetadataExporterEditorModule, MetadataExporterEditor)

void FMetadataExporterEditorModule::StartupModule()
{
    if (UToolMenus::IsToolMenuUIEnabled())
    {
        UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMetadataExporterEditorModule::RegisterMenus));
    }
}

void FMetadataExporterEditorModule::ShutdownModule()
{
    if (UToolMenus::IsToolMenuUIEnabled())
    {
        UToolMenus::UnregisterOwner(this);
    }
}

void FMetadataExporterEditorModule::ExportAssetsToJson()
{
    ResetExport();
    GetGameAssets();
    ExportMetadata<FTextureSerializer>(Assets, ExportedMetadata);
    ExportMetadata<FMeshSerializer>(Assets, ExportedMetadata);
    SaveToJson();
}

void FMetadataExporterEditorModule::RegisterMenus()
{
    UToolMenus* ToolMenus = UToolMenus::Get();
    
    if (!ToolMenus)
    {
        return;
    }

    UToolMenu* Menu = ToolMenus->ExtendMenu("LevelEditor.MainMenu.Tools");

    if (!Menu)
    {
        return;
    }

    FToolMenuSection& Section = Menu->AddSection("MetadataExporter",FText::FromString("Asset Validator"));
    
    const FName InName = "ExportAssetsForValidation";
    const FText Label = FText::FromString("Export Assets for Validation");
    const FText ToolTip = FText::FromString("Export asset metadata to JSON for external validation");
    const FSlateIcon Icon = FSlateIcon();
    const FUIAction Callback = FUIAction(FExecuteAction::CreateRaw(this, &FMetadataExporterEditorModule::ExportAssetsToJson));
    
    Section.AddMenuEntry(InName, Label, ToolTip, Icon, Callback);
}

void FMetadataExporterEditorModule::ResetExport()
{
    ExportedMetadata.Reset();
    OutputString.Reset();
    Assets.Reset();
}

void FMetadataExporterEditorModule::GetGameAssets()
{
    const FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    FARFilter Filter;
    Filter.PackagePaths.Add(FName("/Game"));
    Filter.bRecursivePaths = true;

    AssetRegistry.Get().GetAssets(Filter, Assets);
}

template<typename TSerializer>
void FMetadataExporterEditorModule::ExportMetadata(
    const TArray<FAssetData>& Assets,
    TArray<TSharedPtr<FJsonValue>>& OutExportedMetadata
)
{
    for (const FAssetData& AssetData : Assets)
    {
        TSerializer Serializer(AssetData);

        if (!Serializer.IsValid())
        {
            continue;
        }

        OutExportedMetadata.Add(Serializer.Serialize());
    }
}

void FMetadataExporterEditorModule::SaveToJson()
{
    const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y-%m-%d_%H-%M-%S"));
    const FString FileName = FString::Printf(TEXT("Metadata_%s.json"), *Timestamp);
    const FString ExportPath = FPaths::ProjectSavedDir() / TEXT("Exports");
    const FString OutputPath = ExportPath / FileName;
    IFileManager::Get().MakeDirectory(*FPaths::GetPath(OutputPath), true);
    
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(ExportedMetadata, Writer);
    
    FFileHelper::SaveStringToFile(OutputString, *OutputPath);

    UE_LOG(LogTemp, Log, TEXT("[MetadataExporter] Exported %d asset metadata to %s"), ExportedMetadata.Num(), *OutputPath);
    
    const FText Message = FText::FromString(
        TEXT("Metadata export completed successfully.\n\n")
        TEXT("Would you like to open the exports folder?")
    );

    const EAppReturnType::Type Result =
        FMessageDialog::Open(
            EAppMsgType::YesNo,
            Message
        );
    
    const FString FullExportPath = FPaths::ConvertRelativePathToFull(ExportPath);
    
    if (Result == EAppReturnType::Yes)
    {
        FPlatformProcess::ExploreFolder(*FullExportPath);
    }
}
