// Fill out your copyright notice in the Description page of Project Settings.

#include "G_GameModeBase.h"

#include "UI/GdHUD.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Manager/LoginManager.h"
#include "Manager/UIManager.h"


//void AG_GameModeBase::BeginPlay()
//{
//}
//
//AG_GameModeBase::AG_GameModeBase()
//{
//	HUDClass = AGdHUD::StaticClass();
//}
//
//
//void AG_GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
//{
//	Super::InitGame(MapName, Options, ErrorMessage);
//	// 延后到下一帧启动登录流程
//	if (GetWorld())
//	{
//		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HanlePlayerLogin);
//	}
//}
//
//void AG_GameModeBase::HanlePlayerLogin()
//{
//	UUIManager::GetUIManagerInstance()->ShowLoginUI(GetWorld());
//	ULoginManager* LoginManager = NewObject<ULoginManager>();
//	if (LoginManager)
//	{
//		// 启动登录流程，使用测试账号
//		LoginManager->StartLoginProcess(TEXT("admin"), TEXT("admin123"), nullptr);
//	}
//}