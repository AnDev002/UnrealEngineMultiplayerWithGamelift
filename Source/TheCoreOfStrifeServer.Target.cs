using UnrealBuildTool;
using System.Collections.Generic;

public class TheCoreOfStrifeServerTarget : TargetRules
{
    public TheCoreOfStrifeServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        ExtraModuleNames.Add("TCOS_Multiplayer");
        ExtraModuleNames.Add("DedicatedServers");
    }
}
