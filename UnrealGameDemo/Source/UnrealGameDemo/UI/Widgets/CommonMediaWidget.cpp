// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonMediaWidget.h"
void UCommonMediaWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MediaPlayer)
	{
		// 防止重复绑定
		MediaPlayer->OnEndReached.RemoveAll(this);
		MediaPlayer->OnEndReached.AddDynamic(this, &UCommonMediaWidget::OnMediaFinished);

	}
}


void UCommonMediaWidget::NativeDestruct()
{
	if (MediaPlayer)
	{
		MediaPlayer->OnEndReached.RemoveAll(this);
	}
	Super::NativeDestruct();
}

void UCommonMediaWidget::OnMediaFinished()
{
	UE_LOG(LogTemp, Log, TEXT("[CommonMediaWidget] 视频播放结束"));

	//通知上层系统（LoginSubsystem / UISubsystem）


	/**delegate3.向外广播 */
	OnMediaPlayFinished.Broadcast();
}