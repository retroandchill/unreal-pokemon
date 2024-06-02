using UnrealBuildTool;

public class CommonMathTests : ModuleRules
{
    public CommonMathTests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "CommonMath",
                "AutomationTestHelpers"
            }
        );

        bUseRTTI = true;
    }
}