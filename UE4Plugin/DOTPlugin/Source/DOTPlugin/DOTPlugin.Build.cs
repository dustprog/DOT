// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class DOTPlugin : ModuleRules
	{
        public DOTPlugin(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {			
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					"DOTPlugin/Private",
					// ... add other private include paths required here ...
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
                    "InputCore",
					"Engine",
					"Slate",
					"SlateCore",
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					// ... add private dependencies that you statically link with here ...
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
		}
	}
}