// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class Boost : ModuleRules
{
	public Boost(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {  });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string BasePath = Path.Combine(ModuleDirectory, "../../ThirdParty/Boost/x64-windows");
			PublicIncludePaths.AddRange(new string[] {
				Path.Combine(BasePath, "include")
			});

			PublicAdditionalLibraries.AddRange(new string[] {
				Path.Combine(BasePath, "lib/boost_system-vc140-mt.lib")
			});
			PublicRuntimeLibraryPaths.AddRange(new string[] { Path.Combine(BasePath, "bin/") });
			PublicDefinitions.AddRange(new string[] { 
				"BOOST_ASIO_STANDALONE","_WEBSOCKETPP_CPP11_TYPE_TRAITS_"}); 
		}
		CppStandard = CppStandardVersion.Cpp17;
		bUseRTTI = true;
		bEnableExceptions = true;
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
