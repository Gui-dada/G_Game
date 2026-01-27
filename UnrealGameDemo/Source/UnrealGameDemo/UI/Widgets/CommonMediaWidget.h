// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/G_CommonUserWidget.h"
#include "MediaPlayer.h"
#include "FileMediaSource.h"
#include "CommonMediaWidget.generated.h"


/**delegate1. 视频播放结束委托 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMediaPlayFinished);

UCLASS()
class UNREALGAMEDEMO_API UCommonMediaWidget : public UG_CommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GCUMedia")
	UMediaPlayer* MediaPlayer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GCUMedia")
	UFileMediaSource* FileMediaSource;

	/**delegate2. 对外广播：视频播放结束 */
	UPROPERTY(BlueprintAssignable, Category = "GCUMedia")
	FOnMediaPlayFinished OnMediaPlayFinished;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void OnMediaFinished();
};
