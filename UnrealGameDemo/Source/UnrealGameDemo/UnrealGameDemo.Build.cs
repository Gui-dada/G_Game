// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealGameDemo : ModuleRules
{
	public UnrealGameDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // 添加必要的模块
        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore" ,
            "UMG",
            "EnhancedInput",
            "HTTP",
            "Json",
            "JsonUtilities",
            "OnlineSubsystem",
            "OnlineSubsystemUtils"
        });
        

        PrivateDependencyModuleNames.AddRange(new string[] {
            "CommonUI",
        });
        PrivateIncludePaths.AddRange(new string[] {
            "UnrealGameDemo/UI",
            "UnrealGameDemo/UI/Widgets",
            "UnrealGameDemo/GameModes",
            "UnrealGameDemo/Types",
            //"UnrealGameDemo/Player",
        });
        PublicIncludePaths.Add(ModuleDirectory);


        // Uncomment if you are using Slate UI
        // 取消注释如果您正在使用Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // 取消注释如果您正在使用在线功能
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        // 要包含OnlineSubsystemSteam，请将其添加到您的uproject文件的插件部分，并将Enabled属性设置为true
    }
}
