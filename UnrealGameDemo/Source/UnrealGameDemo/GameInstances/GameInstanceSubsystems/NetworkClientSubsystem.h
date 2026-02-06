// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "NetworkClientSubsystem.generated.h"

//class APlayerController;
/**
 * 
 */
UCLASS()
class UNREALGAMEDEMO_API UNetworkClientSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// 启动登录请求
	void StartLoginRequest(const FString& Username, const FString& Password, APlayerController* PlayerController);

	// 登录请求的响应回调
	void OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, APlayerController* PlayerController);

private:
	// 发送登录请求
	bool SendLoginRequest(const FString& Username, const FString& Password, APlayerController* PlayerController);

	void SaveLoginDataToConfig(const FString& Token, const FString& UserId, const FString& Username);
	UFUNCTION(BlueprintCallable, Category = "Network|Login")
	void NotifyLoginSuccess(const FString& Token, const FString& UserId,
		const FString& Username, APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, Category = "Network|Login")
	void NotifyLoginFailed(const FString& ErrorMessage, APlayerController* PlayerController);

	// 清除无效的登录数据
	void ClearInvalidLoginData();
};
