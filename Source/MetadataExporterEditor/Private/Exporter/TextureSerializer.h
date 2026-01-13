#pragma once

#include "AssetSerializer.h"

class FTextureSerializer final : public FAssetSerializer
{
public:
	explicit FTextureSerializer(const FAssetData& InAssetData) : FAssetSerializer(InAssetData)
	{
		Texture = Cast<UTexture2D>(InAssetData.GetAsset());
	}

protected:
	virtual void WriteMetadataJson(FJsonObject& OutJson) const override
	{
		OutJson.SetNumberField(TEXT("Image.Height"), Texture->GetSizeY());
		OutJson.SetNumberField(TEXT("Image.Width"), Texture->GetSizeX());
	}

	virtual EAssetType GetAssetType() const override
	{
		return EAssetType::Image;
	}
public:
	virtual bool IsValid() const override
	{
		return Texture != nullptr;
	}

protected:
	virtual const int64 GetSizeInBytes() const override
	{
		return Texture->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal);
	}

private:
	UTexture2D* Texture;
};
