// Fill out your copyright notice in the Description page of Project Settings.

#include "G_GameModeBase.h"
#include "Character/G_Character.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"
//#include "OnlineSubsystem.h"
//#include "Interfaces/OnlineIdentityInterface.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AG_GameModeBase::AG_GameModeBase()
{
}

const UG_PawnData* AG_GameModeBase::GetPawnDataForController(const AController* InController) const
{
	return nullptr;
}

void AG_GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp, Warning, TEXT("1111 InitGame called, starting login process 初始化游戏，启动登录流程"));


	// 延后到下一帧启动登录流程
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HanlePlayerLoginInfo);
	}
}

void AG_GameModeBase::HanlePlayerLoginInfo()
{
	// 示例：从 Options 或配置读取账号信息；这里为演示直接用 Options（格式 self:user=xxx;pass=yyy）
	FString Username = TEXT("testuser");
	FString Password = TEXT("testpassword");

	// 如果你把用户名/密码放在 Options，解析 Options 并赋值 username/password
	// Start async login to Spring Boot
	TryDedicatedServerLogin(nullptr, FUniqueNetIdRepl(), FString::Printf(TEXT("username=%s;password=%s"), *Username, *Password));
}

bool AG_GameModeBase::TryDedicatedServerLogin(AController* NewPlayer, const FUniqueNetIdRepl& UniqueId, const FString& Options)
{
	// 构造 REST 请求到 Spring Boot 的登录接口（示例地址，请替换为你的后端地址）
	const FString Url = TEXT("http://127.0.0.1:8080/api/auth/login");

	// TODO: 从 Options 或配置中安全地取得用户名/密码；下面演示静态示例（不可用于生产）
	FString Username = TEXT("admin");
	FString Password = TEXT("admin123");

	// 创建 JSON 载荷
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

	// 绑定回调（注意捕获 this 的弱指针避免生命周期问题）
	Request->OnProcessRequestComplete().BindUObject(this, &AG_GameModeBase::OnSpringLoginResponse, NewPlayer);

	// 发送请求（异步）
	if (!Request->ProcessRequest())
	{
		UE_LOG(LogTemp, Warning, TEXT("1111 Failed to start HTTP login request,启动 HTTP 登录请求失败"));
		return false;
	}

	// 请求已发出（结果在回调处理）
	return true;
}

void AG_GameModeBase::OnSpringLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, AController* NewPlayer)
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
		UE_LOG(LogTemp, Warning, TEXT("1111 Authentication failed，身份验证失败: %s"), *mes);
		return;
	}

	const FString Token = JsonResponse->GetStringField(TEXT("token"));
	const FString UserId = JsonResponse->GetStringField(TEXT("userId"));

	UE_LOG(LogTemp, Log, TEXT("1111 Spring login success.登陆成功  userId=%s tokenLen=%d"), *UserId, Token.Len());


	//// 示例：如果有 OnlineSubsystem，可以把 token 作为凭证传递给 Identity 接口
	//IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	//if (OnlineSub)
	//{
	//	IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
	//	if (Identity.IsValid())
	//	{
	//		int32 LocalUserNum = 0; // 本地玩家索引，按需调整
	//		// 类型字段可自定义，例如 "jwt"、"external"。后端或 OSS 需要识别此种类型。
	//		FOnlineAccountCredentials Creds(TEXT("jwt"), UserId, Token);
	//		bool bLoginStarted = Identity->Login(LocalUserNum, Creds);
	//		UE_LOG(LogTemp, Log, TEXT("Identity->Login invoked: %d"), bLoginStarted ? 1 : 0);
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("OnlineIdentity interface not available"));
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("No OnlineSubsystem present; handle token locally"));
	//	// 你可以在这里把 token 保存到服务器侧会话数据结构，或在 PlayerState 中记录已认证用户
	//}
}
