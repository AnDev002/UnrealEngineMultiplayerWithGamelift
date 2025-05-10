// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TCOS_Multiplayer : ModuleRules
{
	public TCOS_Multiplayer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities", "GameplayTasks", "GameplayTags", "UMG", "Niagara", "DedicatedServers" });
	}
}
