// Fill out your copyright notice in the Description page of Project Settings.


#include "UISubsystem.h"
#include "G_CommonUserWidget.h"
#include <MediaPlayer.h>

void UUISubsystem::ShowUI(FString Path)
{
	// 当前有视频播放，禁止切换UI
	if (isVedioPlaying()) {
		UE_LOG(LogTemp, Warning, TEXT("Video is playing, cannot switch UI."));
		return;
	}
	CurrentUIWidget = CreateWidget<UG_CommonUserWidget>(GetWorld(), LoadClass<UG_CommonUserWidget>(nullptr, *Path));
	if (CurrentUIWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("UIWidget create sucessful!"));
		CurrentUIWidget->AddToViewport();

		// 设置UI的输入模式为UIOnly
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController) {
			SetInputModeToUIOnly(PlayerController);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not be created UIWidget sucessful!"))
	}
}
// 在 HideUI 方法中使用 CurrentUIWidget
void UUISubsystem::HideUI()
{
	if (CurrentUIWidget) {
		CurrentUIWidget->RemoveFromParent();
		CurrentUIWidget = nullptr;
	}

	// 恢复游戏输入模式
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		SetInputModeToGameOnly(PlayerController);
	}
	
}

FInputModeUIOnly UUISubsystem::GetUIInputMode()
{
	return FInputModeUIOnly();
}

void UUISubsystem::SetInputModeToUIOnly(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode); // 设置为UI输入模式
		PlayerController->bShowMouseCursor = true; // 显示鼠标光标
	}
}


void UUISubsystem::SetInputModeToGameOnly(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode); // 设置为游戏输入模式
		PlayerController->bShowMouseCursor = false; // 隐藏鼠标光标
	}
}


bool UUISubsystem::isVedioPlaying() const
{
	if (!CurrentUIWidget) return false;

	//使用反射查找MediaPlayer变量
	FProperty* MediaPlayerProp = CurrentUIWidget->GetClass()->FindPropertyByName(TEXT("MediaPlayer"));
	//if (!MediaPlayerProp)
	//{
	//	// 尝试其他常见名称
	//	MediaPlayerProp = CurrentUIWidget->GetClass()->FindPropertyByName(TEXT("VideoPlayer"));
	//}

	if (MediaPlayerProp && MediaPlayerProp->IsA<FObjectProperty>())
	{
		FObjectProperty* ObjectProp = CastField<FObjectProperty>(MediaPlayerProp);

		// 确保是MediaPlayer类型
		if (ObjectProp->PropertyClass->IsChildOf(UMediaPlayer::StaticClass()))
		{
			// 获取变量值
			UObject* MediaPlayerObj = ObjectProp->GetObjectPropertyValue_InContainer(CurrentUIWidget);
			if (UMediaPlayer* MediaPlayer = Cast<UMediaPlayer>(MediaPlayerObj))
			{
				return MediaPlayer->IsPlaying();
			}
		}
	}

	return false;
}