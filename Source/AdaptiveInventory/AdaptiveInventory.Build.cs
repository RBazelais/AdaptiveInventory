// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AdaptiveInventory : ModuleRules
{
	public AdaptiveInventory(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AdaptiveInventory",
			"AdaptiveInventory/Variant_Platforming",
			"AdaptiveInventory/Variant_Platforming/Animation",
			"AdaptiveInventory/Variant_Combat",
			"AdaptiveInventory/Variant_Combat/AI",
			"AdaptiveInventory/Variant_Combat/Animation",
			"AdaptiveInventory/Variant_Combat/Gameplay",
			"AdaptiveInventory/Variant_Combat/Interfaces",
			"AdaptiveInventory/Variant_Combat/UI",
			"AdaptiveInventory/Variant_SideScrolling",
			"AdaptiveInventory/Variant_SideScrolling/AI",
			"AdaptiveInventory/Variant_SideScrolling/Gameplay",
			"AdaptiveInventory/Variant_SideScrolling/Interfaces",
			"AdaptiveInventory/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
