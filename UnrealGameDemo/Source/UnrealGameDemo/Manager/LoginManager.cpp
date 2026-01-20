// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/LoginManager.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "Player/G_PlayerController.h"
//#include "OnlineSubsystem.h"
//#include "Interfaces/OnlineIdentityInterface.h"

ULoginManager::ULoginManager()
{
}

void ULoginManager::StartLoginProcess(const FString& Username, const FString& Password, APlayerController* PlayerController)
{
	// 发送登录请求
	SendLoginRequest(Username, Password, PlayerController);
}

void ULoginManager::OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, APlayerController* PlayerController)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("1111 Login HTTP request failed or no response，登录 HTTP 请求失败或无响应"));
		return;
	}
	int32 ResponseCode = Response->GetResponseCode();
	FString ResponseStr = Response->GetContentAsString();

	if (ResponseCode != 200)
	{
		UE_LOG(LogTemp, Warning, TEXT("1111 Login failed, HTTP %d: %s"), ResponseCode, *ResponseStr);
		return;
	}

	// 解析 JSON 响应，期望 { "success": true, "token":"...", "userId":"..." }
	TSharedPtr<FJsonObject> JsonResponse;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
	if (!FJsonSerializer::Deserialize(Reader, JsonResponse) || !JsonResponse.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("1111 Failed to parse login response JSON，登录响应 JSON 解析失败"));
		return;
	}

	bool bSuccess = JsonResponse->GetBoolField(TEXT("success"));
	FString mes = JsonResponse->GetStringField(TEXT("message"));
	if (!bSuccess)
	{
		FString Err = JsonResponse->HasField(TEXT("error")) ? JsonResponse->GetStringField(TEXT("error")) : TEXT("Unknown");
		UE_LOG(LogTemp, Warning, TEXT("1111 Authentication failed，身份验证失败: %s"), *Err);
		UE_LOG(LogTemp, Warning, TEXT("1111 Authentication failed，返回信息: %s"), *mes);
		return;
	}

	const FString Token = JsonResponse->GetStringField(TEXT("token"));
	const FString UserId = JsonResponse->GetStringField(TEXT("userId"));

	UE_LOG(LogTemp, Log, TEXT("1111 Spring login success.登陆成功  userId=%s tokenLen=%d"), *UserId, Token.Len());
}

// 修改 SendLoginRequest 方法，增加 PlayerController 参数
bool ULoginManager::SendLoginRequest(const FString& Username, const FString& Password, APlayerController* PlayerController)
{
	// 构造 REST 请求到 Spring Boot 的登录接口
	const FString Url = TEXT("http://127.0.0.1:8080/api/auth/login");

	// 创建 JSON 请求体
	TSharedPtr<FJsonObject> JsonObj = MakeShared<FJsonObject>();
	JsonObj->SetStringField(TEXT("username"), Username);
	JsonObj->SetStringField(TEXT("password"), Password);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);

	// 创建 HTTP 请求
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(RequestBody);

	// 绑定回调，传递 PlayerController
	Request->OnProcessRequestComplete().BindUObject(this, &ULoginManager::OnLoginResponse, PlayerController);

	// 发送请求（异步）
	if (!Request->ProcessRequest())
	{
		UE_LOG(LogTemp, Warning, TEXT("1111 发动 HTTP 登录请求失败"));
		return false;
	}

	// 请求已发出（结果在回调处理）
	return true;
}
