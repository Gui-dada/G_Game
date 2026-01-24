// Fill out your copyright notice in the Description page of Project Settings.


#include "UISubsystem.h"
#include "G_CommonUserWidget.h"

void UUISubsystem::ShowUI(FString Path)
{
	// 先隐藏当前UI
	HideUI();
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
