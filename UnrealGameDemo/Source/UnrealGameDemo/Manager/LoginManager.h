// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <HttpModule.h>
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "LoginManager.generated.h"


/**
 * 
 */
UCLASS()
class UNREALGAMEDEMO_API ULoginManager : public UObject
{
	GENERATED_BODY()

public:
	ULoginManager();

	// 启动登录流程
	void StartLoginProcess(const FString& Username, const FString& Password, APlayerController* PlayerController);

private:
	
	// 登录请求回调
	void OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, APlayerController* PlayerController);

	// 发送登录请求
	bool SendLoginRequest(const FString& Username, const FString& Password, APlayerController* PlayerController);

};
