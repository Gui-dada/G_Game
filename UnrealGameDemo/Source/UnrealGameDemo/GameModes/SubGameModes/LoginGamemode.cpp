// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGamemode.h"
#include "UISubsystem.h"
#include "MediaPlayer.h"
#include "MediaSource.h" 
#include "NetworkClientSubsystem.h"
#include "LoginSubsystem.h"

void ALoginGamemode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("[LoginGameMode] 登录场景开始"));

	// 获取UISubsystem
	//UUISubsystem* UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISubsystem>();
	//if (UI)
	//{
	//	// 绑定
	//	UI->OnVideoEnd.AddDynamic(this, &ALoginGamemode::HandleVideoEnd);

	//	// 触发
	//	UI->TestTriggerVideo();
	//}

    // 延迟一帧确保所有系统初始化
    GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
        {
            StartLoginFlow();
        });
}

void ALoginGamemode::StartLoginFlow()
{
    // 获取玩家控制器
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("[LoginGameMode] 无玩家控制器"));
        return;
    }
    // 2获取LoginSubsystem并开始登录流程
    ULoginSubsystem* LoginSystem = GetWorld()->GetGameInstance()->GetSubsystem<ULoginSubsystem>();
    if (LoginSystem)
    {
        // LoginSubsystem会处理：自动登录检查、显示登录界面等
        LoginSystem->StartLoginProcess(PC);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[LoginGameMode] 无法获取LoginSubsystem"));
    }
}

//void ALoginGamemode::HandleVideoEnd(bool bSuccess)
//{
//	UE_LOG(LogTemp, Log, TEXT("delegate: 收到视频结束事件"));
//}