// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "GameStateManger.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMEDEMO_API UGameStateManger : public UObject
{
	GENERATED_BODY()
public:
	UGameStateManger(const FObjectInitializer& ObjectInitializer);

	static UGameStateManger* GetGameStateMangerInstance();

	static void DestroyGameStateMangerInstance();

public:
	// 全局游戏状态

private:
	static UGameStateManger* GameStateMangerInstance;

};
