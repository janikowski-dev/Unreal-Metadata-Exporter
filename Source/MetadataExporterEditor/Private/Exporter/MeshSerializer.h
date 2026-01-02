#pragma once

#include "AssetSerializer.h"

class FMeshSerializer final : public FAssetSerializer
{
public:
	explicit FMeshSerializer(const FAssetData& InAssetData) : FAssetSerializer(InAssetData)
	{
		Mesh = Cast<UStaticMesh>(InAssetData.GetAsset());
	}

	virtual bool IsValid() const override
	{
		return Mesh && Mesh->GetRenderData() && Mesh->GetRenderData()->LODResources.Num() > 0;
	}

protected:
	virtual void WriteMetadataJson(FJsonObject& OutJson) const override
	{
		const FStaticMeshLODResources& LOD = Mesh->GetRenderData()->LODResources[0];
		OutJson.SetNumberField(TEXT("Mesh.VertexCount"), LOD.GetNumVertices());
		OutJson.SetNumberField(TEXT("Mesh.TriangleCount"), LOD.GetNumTriangles());
		OutJson.SetNumberField(TEXT("UV.MapCount"), LOD.GetNumTexCoords());
	}

	virtual EAssetType GetAssetType() const override
	{
		return EAssetType::Mesh;
	}

	virtual const int64 GetSizeInBytes() const override
	{
		return Mesh->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal);
	}

private:
	UStaticMesh* Mesh;
};
