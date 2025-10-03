using UnrealBuildTool;

public class UnrealSharpTestUtils : ModuleRules
{
    public UnrealSharpTestUtils(ReadOnlyTargetRules target)
        : base(target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(["Core", "Engine", "UnrealSharpBinds"]);

        PrivateDependencyModuleNames.AddRange(["CoreUObject", "Engine", "Slate", "SlateCore", "EngineSettings"]);
    }
}
