// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/G_CommonUserWidget.h"
#include "MediaPlayer.h"
#include "FileMediaSource.h"
#include "CommonMediaWidget.generated.h"
UCLASS()
class UNREALGAMEDEMO_API UCommonMediaWidget : public UG_CommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GCUMedia")
	UMediaPlayer* MediaPlayer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GCUMedia")
	UFileMediaSource* FileMediaSource;
};
