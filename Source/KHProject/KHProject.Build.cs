// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KHProject : ModuleRules
{
	public KHProject(ReadOnlyTargetRules Target) : base(Target)
	{
		
		bLegacyPublicIncludePaths = true; 
		bLegacyParentIncludePaths = true;
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule","OnlineSubsystem","OnlineSubsystemUtils","OnlineSubsystemNull" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","GameplayAbilities","GameplayTags","AnimGraphRuntime","GameplayTasks","UMG","Slate","SlateCore" });
		
		PublicIncludePaths.AddRange(
			new string[] {
				"KHProject/Character", // 예: Source/KHProject/GAS 폴더
				"KHProject/GAS"   // 예: Source/KHProject/AI 폴더
			}
		);
		
	}
}
