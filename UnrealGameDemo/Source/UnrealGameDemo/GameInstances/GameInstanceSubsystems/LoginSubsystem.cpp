// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginSubsystem.h"
#include "ConfigSubsystem.h"
#include "NetworkClientSubsystem.h"
#include "UISubsystem.h"
#include "Kismet/GameplayStatics.h"

void ULoginSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Log, TEXT("[LoginSubsystem] 初始化完成"));
}

// ========== 登录流程控制 ==========

void ULoginSubsystem::StartLoginProcess(APlayerController* PlayerController)
{
	//UISubsystem->ShowUI(TEXT("/Game/UI/LoginWidgets/WB_Start.WB_Start_C"));
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("[LoginSubsystem] 玩家控制器为空"));
		return;
	}

	CurrentPlayerController = PlayerController;
	bIsProcessingLogin = false;

	UE_LOG(LogTemp, Log, TEXT("[LoginSubsystem] 开始登录流程"));

	ShowLoginLoading(PlayerController);

}

bool ULoginSubsystem::TryAutoLogin(APlayerController* PlayerController)
{
	if (bIsProcessingLogin) return false;

	SetLoginState(ELoginState::CheckingAuto);

	UE_LOG(LogTemp, Log, TEXT("[LoginSubsystem] 尝试自动登录"));

	// 检查本地是否有有效的登录数据
	UConfigSubsystem* ConfigSystem = GetConfigSubsystem();
	if (!ConfigSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("[LoginSubsystem] 无法获取ConfigSubsystem"));
		return false;
	}

	if (ConfigSystem->HasValidLoginData())
	{
		FString Username = ConfigSystem->GetCurrentUsername();
		UE_LOG(LogTemp, Log, TEXT("[LoginSubsystem] 检测到有效登录数据: %s"), *Username);

		// 显示加载界面
		ShowLoginLoading(PlayerController, TEXT("自动登录中..."));

		// 模拟网络验证延迟（实际应该调用网络验证）



		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("[LoginSubsystem] 无有效登录数据，需要手动登录"));
	return false;
}

void ULoginSubsystem::ShowLoginUI(APlayerController* PlayerController)
{
	if (!PlayerController) return;

	SetLoginState(ELoginState::ShowingUI);

	UE_LOG(LogTemp, Log, TEXT("[LoginSubsystem] 显示登录界面"));

	// 通过UISubsystem显示登录界面
	UUISubsystem* UISystem = GetUISubsystem();
	if (UISystem)
	{
		// 显示登录UI
		///Script/UMGEditor.WidgetBlueprint'/Game/UI/LoginWidgets/WB_Login.WB_Login'
		UISystem->ShowCommonUI(TEXT("/Game/UI/LoginWidgets/WB_Login.WB_Login_C"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[LoginSubsystem] 无法获取UISubsystem"));

		// 应急：直接使用默认账号登录
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, PlayerController]()
			{
				ManualLogin(TEXT("admin"), TEXT("admin123"), PlayerController);
			});
	}
}

void ULoginSubsystem::ManualLogin(const FString& Username, const FString& Password, APlayerController* PlayerController)
{
	if (Username.IsEmpty() || Password.IsEmpty() || !PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("[LoginSubsystem] 登录参数无效"));
		return;
	}

	if (bIsProcessingLogin)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LoginSubsystem] 已有登录请求在处理中"));
		return;
	}

	SetLoginState(ELoginState::LoggingIn);
	bIsProcessingLogin = true;

	UE_LOG(LogTemp, Log, TEXT("[LoginSubsystem] 开始手动登录: %s"), *Username);

	// 显示加载界面
	ShowLoginLoading(PlayerController, TEXT("登录中..."));

	// 通过网络子系统发送登录请求
	UNetworkClientSubsystem* NetworkSystem = GetNetworkSubsystem();
	if (NetworkSystem)
	{
		NetworkSystem->StartLoginRequest(Username, Password, PlayerController);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[LoginSubsystem] 无法获取NetworkClientSubsystem"));

		// 模拟登录失败
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, PlayerController]()
			{
				OnLoginFailed(TEXT("网络系统不可用"), PlayerController);
			});
	}
}

void ULoginSubsystem::OnLoginSuccess(const FString& Token, const FString& UserId,
	const FString& Username, APlayerController* PlayerController)
{
	SetLoginState(ELoginState::Success);
	bIsProcessingLogin = false;

	UE_LOG(LogTemp, Log, TEXT("[LoginSubsystem] 登录成功: %s (ID: %s)"), *Username, *UserId);

	// 保存登录数据到配置
	UConfigSubsystem* ConfigSystem = GetConfigSubsystem();
	if (ConfigSystem)
	{
		ConfigSystem->SaveLoginData(Token, UserId, Username, 7, true);
	}

	// 隐藏登录界面
	HideLoginUI(PlayerController);

	// 显示成功提示
	ShowLoginLoading(PlayerController, TEXT("登录成功，跳转中..."));

	// 延迟切换到主菜单
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, PlayerController, Username]()
		{
			SwitchToMainMenu(PlayerController);

			// 广播成功事件
			OnLoginSuccessDelegate.Broadcast(Username);

		}, 1.5f, false);
}

void ULoginSubsystem::OnLoginFailed(const FString& ErrorMessage, APlayerController* PlayerController)
{
	SetLoginState(ELoginState::Failed);
	bIsProcessingLogin = false;

	UE_LOG(LogTemp, Error, TEXT("[LoginSubsystem] 登录失败: %s"), *ErrorMessage);

	// 清除无效的登录数据
	UConfigSubsystem* ConfigSystem = GetConfigSubsystem();
	if (ConfigSystem)
	{
		ConfigSystem->ClearLoginData();
	}

	// 隐藏加载界面
	UUISubsystem* UISystem = GetUISubsystem();
	if (UISystem)
	{
		// UISystem->HideLoading();
	}

	// 显示错误信息
	if (UISystem)
	{
		// UISystem->ShowErrorMessage(TEXT("登录失败"), ErrorMessage);
	}

	// 重新显示登录界面
	GetWorld()->GetTimerManager().SetTimerForNextTick([this, PlayerController]()
		{
			ShowLoginUI(PlayerController);
		});

	// 广播失败事件
	OnLoginFailedDelegate.Broadcast(ErrorMessage);
}

void ULoginSubsystem::Logout()
{
	UE_LOG(LogTemp, Log, TEXT("[LoginSubsystem] 用户登出"));

	// 清除登录数据
	UConfigSubsystem* ConfigSystem = GetConfigSubsystem();
	if (ConfigSystem)
	{
		ConfigSystem->ClearLoginData();
	}

	// 重置状态
	SetLoginState(ELoginState::None);
	CurrentPlayerController = nullptr;
	bIsProcessingLogin = false;
}

// ========== 状态查询 ==========

bool ULoginSubsystem::IsLoggedIn() const
{
	UConfigSubsystem* ConfigSystem = GetConfigSubsystem();
	return ConfigSystem ? ConfigSystem->HasValidLoginData() : false;
}

FString ULoginSubsystem::GetCurrentUsername() const
{
	UConfigSubsystem* ConfigSystem = GetConfigSubsystem();
	return ConfigSystem ? ConfigSystem->GetCurrentUsername() : TEXT("");
}

FString ULoginSubsystem::GetCurrentUserId() const
{
	UConfigSubsystem* ConfigSystem = GetConfigSubsystem();
	return ConfigSystem ? ConfigSystem->GetCurrentUserId() : TEXT("");
}

// ========== 私有方法 ==========

void ULoginSubsystem::SetLoginState(ELoginState NewState)
{
	if (CurrentLoginState != NewState)
	{
		CurrentLoginState = NewState;
		OnLoginStateChanged.Broadcast();

		UE_LOG(LogTemp, Log, TEXT("[LoginSubsystem] 状态改变: %s"),
			*UEnum::GetValueAsString(NewState));
	}
}

void ULoginSubsystem::SwitchToMainMenu(APlayerController* PlayerController)
{
	if (!PlayerController) return;

	UE_LOG(LogTemp, Log, TEXT("[LoginSubsystem] 切换到主菜单"));

	// 跳转到主菜单关卡
	UGameplayStatics::OpenLevel(PlayerController, TEXT("MainMenuMap"));
}

void ULoginSubsystem::ShowLoginLoading(APlayerController* PlayerController, const FString& Message)
{
	// 通过UISubsystem显示加载界面
	UUISubsystem* UISystem = GetUISubsystem();
	if(!UISystem)
	{
		UE_LOG(LogTemp, Error, TEXT("[LoginSubsystem] 无法获取UISubsystem"));
		return;
	}
	UISystem->ShowMediaUI("/Game/UI/LoginWidgets/WB_Start.WB_Start_C");
	if (UISystem->MediaWidget != nullptr) {
		/**delegate4 绑定事件 然后觉得方法 */
		UISystem->MediaWidget->OnMediaPlayFinished.AddDynamic(this, &ULoginSubsystem::OnStartMediaFinished);
	}
}

void ULoginSubsystem::HideLoginUI(APlayerController* PlayerController)
{
	// 通过UISubsystem隐藏登录界面
	UUISubsystem* UISystem = GetUISubsystem();
	if (UISystem)
	{
		// UISystem->HideLoginUI();
	}
}

// ========== 获取子系统引用 ==========

UConfigSubsystem* ULoginSubsystem::GetConfigSubsystem() const
{
	UGameInstance* GameInstance = GetGameInstance();
	return GameInstance ? GameInstance->GetSubsystem<UConfigSubsystem>() : nullptr;
}

UNetworkClientSubsystem* ULoginSubsystem::GetNetworkSubsystem() const
{
	UGameInstance* GameInstance = GetGameInstance();
	return GameInstance ? GameInstance->GetSubsystem<UNetworkClientSubsystem>() : nullptr;
}

UUISubsystem* ULoginSubsystem::GetUISubsystem() const
{
	UGameInstance* GameInstance = GetGameInstance();
	return GameInstance ? GameInstance->GetSubsystem<UUISubsystem>() : nullptr;
}

void ULoginSubsystem::OnStartMediaFinished()
{
	UE_LOG(LogTemp, Log, TEXT("[LoginSubsystem] 收到启动视频播放结束回调"));
	UUISubsystem* UISystem = GetUISubsystem();
	if (!UISystem)
	{
		UE_LOG(LogTemp, Error, TEXT("[LoginSubsystem] 无法获取UISubsystem"));
		return;
	}
	UISystem->HideMediaUI();
	ShowLoginUI(CurrentPlayerController);
}
