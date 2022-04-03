// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class WebSocketpp : ModuleRules
{
	public WebSocketpp(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Boost" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string BasePath = Path.Combine(ModuleDirectory, "../../ThirdParty/WebSocketpp/x64-windows");
			PublicIncludePaths.AddRange(new string[] {
				Path.Combine(BasePath, "include")
			});
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
