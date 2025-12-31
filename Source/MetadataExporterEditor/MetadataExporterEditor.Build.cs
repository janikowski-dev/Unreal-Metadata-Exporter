using UnrealBuildTool;

public class MetadataExporterEditor : ModuleRules
{
	public MetadataExporterEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine",
			"AssetRegistry",
			"Json",
			"JsonUtilities"
		]);

		PrivateDependencyModuleNames.AddRange([
			"Slate",
			"SlateCore",
			"EditorSubsystem",
			"UnrealEd",
			"ToolMenus",
			"AssetRegistry"
		]);
	}
}