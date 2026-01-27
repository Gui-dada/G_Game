// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "G_CommonUserWidget.h"
#include "CommonMediaWidget.h"
#include "UISubsystem.generated.h"

/**
 *
 */

UCLASS()
class UNREALGAMEDEMO_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void ShowCommonUI(FString Path);

	void ShowMediaUI(FString Path);

	void HideCommonUI();

	void HideMediaUI();

	// 获取UI的输入模式（如用于控制是否允许玩家与游戏互动）
	FInputModeUIOnly GetUIInputMode();

	// 设置UI的输入模式（一般在显示/隐藏UI时调用）
	void SetInputModeToUIOnly(APlayerController* PlayerController);
	void SetInputModeToGameOnly(APlayerController* PlayerController);

	UG_CommonUserWidget* UIWidget = nullptr;
	UCommonMediaWidget* MediaWidget = nullptr;
};
