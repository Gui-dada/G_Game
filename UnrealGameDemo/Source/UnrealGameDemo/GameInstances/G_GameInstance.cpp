// Fill out your copyright notice in the Description page of Project Settings.


#include "G_GameInstance.h"
#include <UISubsystem.h>
#include <NetworkClientSubsystem.h>
#include <ConfigSubsystem.h>

void UG_GameInstance::Init()
{
	Super::Init();

	/// 初始化 UISubsystem 和 NetworkSubsystem 等
	UUISubsystem* UISubsystem = GetSubsystem<UUISubsystem>();
	UNetworkClientSubsystem* NetworkSubsystem = GetSubsystem<UNetworkClientSubsystem>();
	UConfigSubsystem* ConfigSubsystem = GetSubsystem<UConfigSubsystem>();

	//可以在这里做更多初始化工作，如加载配置、初始化数据等

}
