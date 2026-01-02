#pragma once

#include "AssetType.h"

class FAssetSerializer
{
public:
	explicit FAssetSerializer(const FAssetData& InAssetData)
	{
		Path = InAssetData.GetObjectPathString();
		Name = InAssetData.AssetName.ToString();
	}

	virtual ~FAssetSerializer() = default;

	TSharedPtr<FJsonValue> Serialize() const
	{
		const TSharedPtr<FJsonObject> MetadataJson = MakeShared<FJsonObject>();
		WriteMetadataJson(*MetadataJson);
	 	return MakeShared<FJsonValueObject>(Serialize(MetadataJson));
	}

	virtual bool IsValid() const = 0;
	
protected:
	virtual void WriteMetadataJson(FJsonObject& OutMetadata) const = 0;
	virtual const int64 GetSizeInBytes() const = 0;
    virtual EAssetType GetAssetType() const = 0;

private:
	const FString Source = TEXT("UnrealEngine");
	FString Name;
	FString Path;
	
	TSharedPtr<FJsonObject> Serialize(const TSharedPtr<FJsonObject>& MetadataJson) const
	{
		TSharedPtr<FJsonObject> JsonAsset = MakeShared<FJsonObject>();
		JsonAsset->SetStringField(TEXT("Type"), ToString(GetAssetType()));
		JsonAsset->SetStringField(TEXT("Source"), Source);
		JsonAsset->SetStringField(TEXT("Name"), Name);
		JsonAsset->SetStringField(TEXT("Path"), Path);
		JsonAsset->SetNumberField(TEXT("SizeInBytes"), GetSizeInBytes());
		JsonAsset->SetObjectField(TEXT("Metadata"), MetadataJson);
		return JsonAsset;
	}

	static const TCHAR* ToString(const EAssetType Type)
	{
		switch (Type)
		{
		case EAssetType::Mesh:
			return TEXT("Mesh");
		case EAssetType::Texture:
			return TEXT("Texture");
		}

		return TEXT("Invalid");
	}
};
