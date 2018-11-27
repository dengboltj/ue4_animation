// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AnimationProject : ModuleRules
{
	public AnimationProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
    }
}
