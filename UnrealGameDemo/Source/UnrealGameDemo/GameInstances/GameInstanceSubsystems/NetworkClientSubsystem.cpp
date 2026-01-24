// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkClientSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Player/G_PlayerController.h"

void UNetworkClientSubsystem::StartLoginRequest(const FString& Username, const FString& Password, APlayerController* PlayerController)
{
	// 发送登录请求
	SendLoginRequest(Username, Password, PlayerController);
}

void UNetworkClientSubsystem::OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, APlayerController* PlayerController)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Login HTTP request failed or no response"));
		return;
	}

	int32 ResponseCode = Response->GetResponseCode();
	FString ResponseStr = Response->GetContentAsString();

	if (ResponseCode != 200)
	{
		UE_LOG(LogTemp, Warning, TEXT("Login failed, HTTP %d: %s"), ResponseCode, *ResponseStr);
		return;
	}

	// 解析 JSON 响应，期望 { "success": true, "token":"...", "userId":"..." }
	TSharedPtr<FJsonObject> JsonResponse;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
	if (!FJsonSerializer::Deserialize(Reader, JsonResponse) || !JsonResponse.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to parse login response JSON"));
		return;
	}

	bool bSuccess = JsonResponse->GetBoolField(TEXT("success"));
	FString Message = JsonResponse->GetStringField(TEXT("message"));
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Authentication failed: %s"), *Message);
		return;
	}

	const FString Token = JsonResponse->GetStringField(TEXT("token"));
	const FString UserId = JsonResponse->GetStringField(TEXT("userId"));

	UE_LOG(LogTemp, Log, TEXT("Login successful, userId=%s tokenLen=%d"), *UserId, Token.Len());

}

bool UNetworkClientSubsystem::SendLoginRequest(const FString& Username, const FString& Password, APlayerController* PlayerController)
{
	// 构造 REST 请求到后端登录接口
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
	Request->OnProcessRequestComplete().BindUObject(this, &UNetworkClientSubsystem::OnLoginResponse, PlayerController);

	// 发送请求（异步）
	if (!Request->ProcessRequest())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to send login request"));
		return false;
	}

	// 请求已发出（结果在回调处理）
	return true;
}