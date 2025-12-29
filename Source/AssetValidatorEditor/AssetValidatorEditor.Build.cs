using UnrealBuildTool;

public class AssetValidatorEditor : ModuleRules
{
	public AssetValidatorEditor(ReadOnlyTargetRules Target) : base(Target)
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