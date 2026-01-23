// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGamemode.h"
#include "UISubsystem.h"
#include "MediaPlayer.h"
#include "MediaSource.h" 

void ALoginGamemode::BeginPlay()
{
	Super::BeginPlay();

    // 检查 UUISubsystem 是否有效
    UUISubsystem* UISubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UUISubsystem>();
    if (UISubsystem)
    {
        // 显示 Loading 视频和 UI
        PlayVideoAndShowUI(UISubsystem);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UUISubsystem is nullptr"));
    }
}

// 播放视频并显示 UI
void ALoginGamemode::PlayVideoAndShowUI(UUISubsystem* UISubsystem)
{
    // 创建 MediaPlayer 实例
    UMediaPlayer* MediaPlayer = NewObject<UMediaPlayer>(this);
    // 获取 UUISubsystem 实例
    UISubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UUISubsystem>();
    if (MediaPlayer)
    {
        // 加载视频资源（路径为 "/Game/Medias/Loading/MP_Loading.MP_Loading"）/Script/MediaAssets.FileMediaSource'/Game/Medias/Loading/FMS_Loading.FMS_Loading'
		UMediaSource* MediaSource = LoadObject<UMediaSource>(nullptr, TEXT("/Game/Medias/Loading/FMS_Loading.FMS_Loading"));
        UISubsystem->ShowUI(TEXT("/Game/UI/Login/WB_Start.WB_Start_C"));///Script/UMGEditor.WidgetBlueprint'/Game/UI/Login/WB_Start.WB_Start'
		if (MediaSource && MediaPlayer->OpenSource(MediaSource))
		{
			// 绑定视频播放完成事件
			MediaPlayer->OnEndReached.AddDynamic(this, &ALoginGamemode::OnVideoEnd);

			// 播放视频
			MediaPlayer->Play();
			UE_LOG(LogTemp, Log, TEXT("Playing video..."));
		}
		else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to  open MediaPlayer or load media source!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create MediaPlayer"));
    }
}

// 视频播放结束后回调此方法
void ALoginGamemode::OnVideoEnd()
{
    // 播放完视频后，显示登录界面等
    UE_LOG(LogTemp, Log, TEXT("Video playback finished. Now show login UI."));
    // 获取 UUISubsystem 实例
    UUISubsystem* UISubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UUISubsystem>();
    if (UISubsystem)
    {
        // 播放视频完成后显示登录 UI
        UISubsystem->ShowUI(TEXT("/Game/UI/LoginWidgets/W_Login.W_Login_C"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UUISubsystem is nullptr when trying to show UI after video."));
    }
}
