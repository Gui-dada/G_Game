// Fill out your copyright notice in the Description page of Project Settings.


#include "UISubsystem.h"
#include "G_CommonUserWidget.h"
#include <MediaPlayer.h>

void UUISubsystem::ShowCommonUI(FString Path)
{
	UIWidget = CreateWidget<UG_CommonUserWidget>(GetWorld(), LoadClass<UG_CommonUserWidget>(nullptr, *Path));
	if (UIWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("UIWidget create sucessful!"));
		UIWidget->AddToViewport();

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
void UUISubsystem::ShowMediaUI(FString Path)
{
	MediaWidget = CreateWidget<UCommonMediaWidget>(GetWorld(), LoadClass<UCommonMediaWidget>(nullptr, *Path));
	if (MediaWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("MediaWidget create sucessful!"));
		MediaWidget->AddToViewport();

		// 设置UI的输入模式为UIOnly
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController) {
			SetInputModeToUIOnly(PlayerController);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not be created MediaWidget sucessful!"))
	}
}
// 在 HideUI 方法中使用 CurrentUIWidget
void UUISubsystem::HideCommonUI()
{
	if(UIWidget)
	{
		UIWidget->RemoveFromParent();
		UIWidget = nullptr;
	}
	// 恢复游戏输入模式
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		SetInputModeToGameOnly(PlayerController);
	}
}

void UUISubsystem::HideMediaUI()
{
	if(MediaWidget)
	{
		MediaWidget->RemoveFromParent();
		MediaWidget = nullptr;
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