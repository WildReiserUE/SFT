using UnrealBuildTool;

public class TDS : ModuleRules
{
	public TDS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameplayTasks","NavigationSystem", "AIModule" });
        PublicIncludePaths.AddRange(new string[]{"TDS", "TDS/Character","TDS/Components","TDS/Weapon", "TDS/AI", "TDS/AI/Services", "TDS/Notifies", "TDS/Items", "TDS/DamageTypes" });
    }
}
