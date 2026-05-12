// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Survival : ModuleRules
{
	public Survival(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"Niagara",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"SlateCore",
			"Water",
			"Landscape",
			"AudioMixer",
			"AudioExtensions",
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "Foliage"});

		PublicIncludePaths.AddRange(new string[] {
			"Survival",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
