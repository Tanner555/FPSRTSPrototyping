// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSRTSPrototype : ModuleRules
{
	public FPSRTSPrototype(TargetInfo Target)
	{
        //PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core","CoreUObject", "Engine", "InputCore", "UMG", "Slate","SlateCore" });
      //  MinFilesUsingPrecompiledHeaderOverride = 1;
      //  bFasterWithoutUnity = true;
    }
}
