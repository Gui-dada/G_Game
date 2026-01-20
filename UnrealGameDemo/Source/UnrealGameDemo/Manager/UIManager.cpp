// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/UIManager.h"


UUIManager* UUIManager::UIManagerInstance = nullptr;

UUIManager::UUIManager()
	:LoginWidgetInstance(nullptr)
{
	// 从路径加载登录界面蓝图类，仅加载一次
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/LoginWidgets/W_Login"));
	if (WidgetClassFinder.Succeeded())
	{
		LoginWidgetClass = WidgetClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("1111 Failed to find LoginWidget class at specified path!"));
	}
}

UUIManager* UUIManager::GetUIManagerInstance()
{
	if (!UIManagerInstance)
	{
		UIManagerInstance = NewObject<UUIManager>();
		UIManagerInstance->AddToRoot(); // 防止垃圾回收
	}
	return UIManagerInstance;
}

void UUIManager::DestroyUIManagerInstance()
{
	if (UIManagerInstance)
	{
		// 移除根，允许垃圾回收
		UIManagerInstance->RemoveFromRoot();
		UIManagerInstance = nullptr;// 防止悬空指针
	}
}

void UUIManager::ShowLoginUI(UWorld* WorldContext)
{
	// 确保WorldContext有效
	if (!WorldContext || !LoginWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("1111  Invalid WorldContext or LoginWidgetClass is null!"));
		return;
	}
	//if (!WorldContext) {
	//	UE_LOG(LogTemp, Warning, TEXT("1111 WorldContext is null!"));
	//	return;
	//}
	//if(!LoginWidgetClass)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("1111 LoginWidgetClass is null!"));
	//	return;
	//}


	// 如果UI实例已经创建，则不再重复创建
	if (!LoginWidgetInstance)
	{
		LoginWidgetInstance = CreateWidget<UUserWidget>(WorldContext, LoginWidgetClass);
		if (LoginWidgetInstance)
		{
			LoginWidgetInstance->AddToViewport();
			UE_LOG(LogTemp, Log, TEXT("1111Login UI displayed successfully."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("1111Failed to create LoginWidgetInstance"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("1111Login UI is already displayed."));
	}
}
