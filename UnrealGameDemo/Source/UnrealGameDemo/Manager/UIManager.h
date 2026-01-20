// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "UIManager.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMEDEMO_API UUIManager : public UObject
{
	GENERATED_BODY()

public:
	UUIManager();
public:
	// 构造函数
	static UUIManager* GetUIManagerInstance();
	/// 销毁单例实例
	static void DestroyUIManagerInstance();


	// 加载并展示登录UI
	void ShowLoginUI(UWorld* WorldContext);

private:
	static UUIManager* UIManagerInstance;

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoginWidgetClass;

	// 登录UI的实例
	UUserWidget* LoginWidgetInstance;

};
