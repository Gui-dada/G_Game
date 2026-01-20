// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/GameStateManger.h"

UGameStateManger::UGameStateManger(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

// 定义静态成员，未定义静态变量导致链接错误
UGameStateManger* UGameStateManger::GameStateMangerInstance = nullptr;

UGameStateManger* UGameStateManger::GetGameStateMangerInstance()
{
	if(!GameStateMangerInstance)
	{
		GameStateMangerInstance = NewObject<UGameStateManger>();
		GameStateMangerInstance->AddToRoot(); // 防止垃圾回收
	}
	return GameStateMangerInstance;
}

void UGameStateManger::DestroyGameStateMangerInstance()
{
	if(GameStateMangerInstance)
	{
		// 移除根，允许垃圾回收
		GameStateMangerInstance->RemoveFromRoot();
		GameStateMangerInstance = nullptr;// 防止悬空指针
	}
}

