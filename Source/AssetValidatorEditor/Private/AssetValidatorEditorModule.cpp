#include "AssetValidatorEditorModule.h"

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

IMPLEMENT_MODULE(FAssetValidatorEditorModule, AssetValidatorEditor)

void FAssetValidatorEditorModule::StartupModule()
{
    if (UToolMenus::IsToolMenuUIEnabled())
    {
        UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAssetValidatorEditorModule::RegisterMenus));
    }
}

void FAssetValidatorEditorModule::ShutdownModule()
{
    if (UToolMenus::IsToolMenuUIEnabled())
    {
        UToolMenus::UnregisterOwner(this);
    }
}

void FAssetValidatorEditorModule::ExportAssetsToJson()
{
    ResetExport();
    GetAllAssets();
    ExportMeshes();
    ExportTextures();
    SaveToJson();
}

void FAssetValidatorEditorModule::RegisterMenus()
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

    FToolMenuSection& Section = Menu->AddSection("AssetValidator",FText::FromString("Asset Validator"));
    
    const FName InName = "ExportAssetsForValidation";
    const FText Label = FText::FromString("Export Assets for Validation");
    const FText ToolTip = FText::FromString("Export asset metadata to JSON for external validation");
    const FSlateIcon Icon = FSlateIcon();
    const FUIAction Callback = FUIAction(FExecuteAction::CreateRaw(this, &FAssetValidatorEditorModule::ExportAssetsToJson));
    
    Section.AddMenuEntry(InName, Label, ToolTip, Icon, Callback);
}

void FAssetValidatorEditorModule::ResetExport()
{
    ExportedAssets.Reset();
    OutputString.Reset();
    Assets.Reset();
}

void FAssetValidatorEditorModule::GetAllAssets()
{
    const FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    AssetRegistry.Get().GetAllAssets(Assets);
}

void FAssetValidatorEditorModule::ExportMeshes()
{
    const FTopLevelAssetPath StaticMeshClassPath = UStaticMesh::StaticClass()->GetClassPathName();

    for (const FAssetData& AssetData : Assets)
    {
        if (AssetData.AssetClassPath != StaticMeshClassPath)
        {
            continue;
        }

        UStaticMesh* Mesh = Cast<UStaticMesh>(AssetData.GetAsset());
        
        if (!Mesh)
        {
            continue;
        }

        int32 VertexCount = 0;
        int32 TriangleCount = 0;

        if (Mesh->GetRenderData() && Mesh->GetRenderData()->LODResources.Num() > 0)
        {
            const FStaticMeshLODResources& LOD = Mesh->GetRenderData()->LODResources[0];
            VertexCount = LOD.GetNumVertices();
            TriangleCount = LOD.GetNumTriangles();
        }

        TSharedPtr<FJsonObject> Metadata = MakeShared<FJsonObject>();
        Metadata->SetNumberField(TEXT("Mesh.VertexCount"), VertexCount);
        Metadata->SetNumberField(TEXT("Mesh.TriangleCount"), TriangleCount);

        TSharedPtr<FJsonObject> JsonAsset = MakeShared<FJsonObject>();
        JsonAsset->SetStringField(TEXT("Type"), TEXT("Mesh"));
        JsonAsset->SetStringField(TEXT("Name"), AssetData.AssetName.ToString());
        JsonAsset->SetStringField(TEXT("Path"), AssetData.GetObjectPathString());
        JsonAsset->SetNumberField(TEXT("SizeInBytes"), Mesh->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal));
        JsonAsset->SetObjectField(TEXT("Metadata"), Metadata);

        ExportedAssets.Add(MakeShared<FJsonValueObject>(JsonAsset));
    }
}

void FAssetValidatorEditorModule::ExportTextures()
{
    const FTopLevelAssetPath TextureClassPath = UTexture2D::StaticClass()->GetClassPathName();
    
    for (const FAssetData& AssetData : Assets)
    {
        if (AssetData.AssetClassPath != TextureClassPath)
        {
            continue;
        }

        UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset());
        
        if (!Texture)
        {
            continue;
        }

        TSharedPtr<FJsonObject> Metadata = MakeShared<FJsonObject>();
        Metadata->SetNumberField(TEXT("Image.Width"), Texture->GetSizeX());
        Metadata->SetNumberField(TEXT("Image.Height"), Texture->GetSizeY());

        TSharedPtr<FJsonObject> JsonAsset = MakeShared<FJsonObject>();
        JsonAsset->SetStringField(TEXT("Type"), TEXT("Image"));
        JsonAsset->SetStringField(TEXT("Name"), AssetData.AssetName.ToString());
        JsonAsset->SetStringField(TEXT("Path"), AssetData.GetObjectPathString());
        JsonAsset->SetNumberField(TEXT("SizeInBytes"), Texture->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal));
        JsonAsset->SetObjectField(TEXT("Metadata"), Metadata);
        
        ExportedAssets.Add(MakeShared<FJsonValueObject>(JsonAsset));
    }
}

void FAssetValidatorEditorModule::SaveToJson()
{
    const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y-%m-%d_%H-%M-%S"));
    const FString FileName = FString::Printf(TEXT("AssetsInfo_%s.json"), *Timestamp);
    const FString OutputPath = FPaths::ProjectSavedDir() / TEXT("Assets") / FileName;
    IFileManager::Get().MakeDirectory(*FPaths::GetPath(OutputPath), true);
    
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(ExportedAssets, Writer);
    
    FFileHelper::SaveStringToFile(OutputString, *OutputPath);
    
    UE_LOG(LogTemp, Log, TEXT("[AssetValidator] Exported %d assets to %s"), ExportedAssets.Num(), *OutputPath);
}
