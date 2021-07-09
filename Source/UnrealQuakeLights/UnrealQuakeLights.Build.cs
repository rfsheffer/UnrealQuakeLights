// Copyright 2020-2021 Solar Storm Interactive. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class UnrealQuakeLights : ModuleRules
{
	public UnrealQuakeLights(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Disable for Non-Developer builds
		//OptimizeCode = CodeOptimization.Never;

        PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
                "CoreUObject",
                "Engine",
			}
		);
	}
}
