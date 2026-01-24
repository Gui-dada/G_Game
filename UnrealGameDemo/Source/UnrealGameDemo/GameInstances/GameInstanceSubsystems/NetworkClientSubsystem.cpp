// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkClientSubsystem.h"
#include "ConfigSubsystem.h"
#include "LoginSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Kismet/GameplayStatics.h"
#include "G_GameInstance.h"

void UNetworkClientSubsystem::StartLoginRequest(const FString& Username, const FString& Password, APlayerController* PlayerController)
{
	// 发送登录请求
	SendLoginRequest(Username, Password, PlayerController);
}

void UNetworkClientSubsystem::OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response,
    bool bWasSuccessful, APlayerController* PlayerController)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Login HTTP request failed or no response"));

        // 通知登录失败
        NotifyLoginFailed(TEXT("网络请求失败"), PlayerController);
        return;
    }

    int32 ResponseCode = Response->GetResponseCode();
    FString ResponseStr = Response->GetContentAsString();

    if (ResponseCode != 200)
    {
        UE_LOG(LogTemp, Warning, TEXT("Login failed, HTTP %d: %s"), ResponseCode, *ResponseStr);

        // 尝试解析错误信息
        FString ErrorMessage = TEXT("登录失败");
        TSharedPtr<FJsonObject> JsonResponse;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);

        if (FJsonSerializer::Deserialize(Reader, JsonResponse) && JsonResponse.IsValid())
        {
            if (JsonResponse->HasField(TEXT("message")))
            {
                ErrorMessage = JsonResponse->GetStringField(TEXT("message"));
            }
        }

        NotifyLoginFailed(ErrorMessage, PlayerController);
        return;
    }

    // 解析 JSON 响应
    TSharedPtr<FJsonObject> JsonResponse;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);

    if (!FJsonSerializer::Deserialize(Reader, JsonResponse) || !JsonResponse.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to parse login response JSON"));
        NotifyLoginFailed(TEXT("响应数据格式错误"), PlayerController);
        return;
    }

    // 检查登录是否成功
    bool bSuccess = false;
    if (JsonResponse->HasField(TEXT("success")))
    {
        bSuccess = JsonResponse->GetBoolField(TEXT("success"));
    }
    else if (JsonResponse->HasField(TEXT("code")))
    {
        int32 Code = JsonResponse->GetIntegerField(TEXT("code"));
        bSuccess = (Code == 200 || Code == 0);
    }

    if (!bSuccess)
    {
        FString ErrorMessage = JsonResponse->HasField(TEXT("message"))
            ? JsonResponse->GetStringField(TEXT("message"))
            : TEXT("登录失败");

        NotifyLoginFailed(ErrorMessage, PlayerController);
        return;
    }

    // 提取Token和用户信息
    FString Token = TEXT("");
    FString UserId = TEXT("");
    FString Username = TEXT("");

    if (JsonResponse->HasField(TEXT("token")))
    {
        Token = JsonResponse->GetStringField(TEXT("token"));
    }

    if (JsonResponse->HasField(TEXT("userId")) || JsonResponse->HasField(TEXT("id")))
    {
        UserId = JsonResponse->HasField(TEXT("userId"))
            ? JsonResponse->GetStringField(TEXT("userId"))
            : JsonResponse->GetStringField(TEXT("id"));
    }

    if (JsonResponse->HasField(TEXT("username")) || JsonResponse->HasField(TEXT("name")))
    {
        Username = JsonResponse->HasField(TEXT("username"))
            ? JsonResponse->GetStringField(TEXT("username"))
            : JsonResponse->GetStringField(TEXT("name"));
    }

    // 如果Token为空，登录失败
    if (Token.IsEmpty())
    {
        NotifyLoginFailed(TEXT("未获取到Token"), PlayerController);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Login successful, userId=%s, username=%s, tokenLen=%d"),
        *UserId, *Username, Token.Len());

    // 保存到本地配置
    SaveLoginDataToConfig(Token, UserId, Username);

    // 通知登录成功
    NotifyLoginSuccess(Token, UserId, Username, PlayerController);
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
// 保存登录数据到配置
void UNetworkClientSubsystem::SaveLoginDataToConfig(const FString& Token, const FString& UserId, const FString& Username)
{
    UG_GameInstance* GameInstance = Cast<UG_GameInstance>(GetGameInstance());
    if (!GameInstance) return;

    UConfigSubsystem* ConfigSystem =GameInstance->GetSubsystem<UConfigSubsystem>();
    if (ConfigSystem)
    {
        // 保存7天有效期，记住登录状态
        ConfigSystem->SaveLoginData(Token, UserId, Username, 7, true);
        UE_LOG(LogTemp, Log, TEXT("登录数据已保存到本地配置"));
    }
}

// 通知登录成功
void UNetworkClientSubsystem::NotifyLoginSuccess(const FString& Token, const FString& UserId,
    const FString& Username, APlayerController* PlayerController)
{
    // 1. 通知LoginSubsystem
    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        ULoginSubsystem* LoginSystem = GameInstance->GetSubsystem<ULoginSubsystem>();
        if (LoginSystem)
        {
            LoginSystem->OnLoginSuccess(Token, UserId, Username, PlayerController);
        }
    }

    // 2. 这里可以添加其他处理，比如UI通知等
}
void UNetworkClientSubsystem::NotifyLoginFailed(const FString& ErrorMessage, APlayerController* PlayerController)
{
    // 1. 通知LoginSubsystem
    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        ULoginSubsystem* LoginSystem = GameInstance->GetSubsystem<ULoginSubsystem>();
        if (LoginSystem)
        {
            LoginSystem->OnLoginFailed(ErrorMessage, PlayerController);
        }
    }

    // 2. 清除本地无效的登录数据（如果有）
    ClearInvalidLoginData();
}

// 清除无效的登录数据
void UNetworkClientSubsystem::ClearInvalidLoginData()
{
    UGameInstance* GameInstance = Cast<UG_GameInstance>(GetGameInstance());
    if (!GameInstance) return;

    UConfigSubsystem* ConfigSystem = GameInstance->GetSubsystem<UConfigSubsystem>();
    if (ConfigSystem && !ConfigSystem->HasValidLoginData())
    {
        // 如果Token无效，清除保存的数据
        ConfigSystem->ClearLoginData();
        UE_LOG(LogTemp, Log, TEXT("已清除无效的登录数据"));
    }
}