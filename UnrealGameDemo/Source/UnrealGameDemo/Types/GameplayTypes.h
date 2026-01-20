// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTypes.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	ENONE			UMETA(DisplayName = "None"),
	EPlaying		UMETA(DisplayName = "Playing"),
	EGameOver		UMETA(DisplayName = "GameOver"),
	EPause			UMETA(DisplayName = "Pause"),
};

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	ENONE			UMETA(DisplayName = "None"),
	EAlive			UMETA(DisplayName = "Alive"),
	EDead			UMETA(DisplayName = "Dead"),
	ERespawn		UMETA(DisplayName = "Respawn"),
};


/**
 * 
 */
UCLASS()
class UNREALGAMEDEMO_API UGameplayTypes : public UDataAsset
{
	GENERATED_BODY()
	
};
