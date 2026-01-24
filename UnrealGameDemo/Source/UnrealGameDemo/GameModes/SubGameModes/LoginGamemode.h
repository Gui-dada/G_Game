// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/G_GameModeBase.h"
#include "UISubsystem.h"
#include "LoginGamemode.generated.h"

/**
 *
 */
UCLASS()
class UNREALGAMEDEMO_API ALoginGamemode : public AG_GameModeBase
{
	GENERATED_BODY()

public:
	// 在游戏开始时初始化UI
	virtual void BeginPlay() override;

	void StartLoginFlow();

};
