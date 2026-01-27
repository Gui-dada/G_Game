// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoginSubsystem.generated.h"

/**
 * 
 */

// 登录状态枚举
UENUM(BlueprintType)
enum class ELoginState : uint8
{
    None            UMETA(DisplayName = "未开始"),
    CheckingAuto    UMETA(DisplayName = "检查自动登录"),
    VedioPlaying    UMETA(DisplayName = "视频播放中"),
    ShowingUI       UMETA(DisplayName = "显示登录界面"),
    LoggingIn       UMETA(DisplayName = "登录中"),
    Success         UMETA(DisplayName = "登录成功"),
    Failed          UMETA(DisplayName = "登录失败"),
};

 // 登录事件委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoginStateChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginSuccess, const FString&, Username);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginFailed, const FString&, ErrorMessage);


UCLASS()
class UNREALGAMEDEMO_API ULoginSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// ========== 生命周期 ==========
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    // ========== 登录流程控制 ==========
    
    // 开始登录流程（LoginGameMode调用）
    UFUNCTION(BlueprintCallable, Category = "Login|Flow")
    void StartLoginProcess(APlayerController* PlayerController);
    
    // 尝试自动登录
    UFUNCTION(BlueprintCallable, Category = "Login|Flow")
    bool TryAutoLogin(APlayerController* PlayerController);
    
    // 显示登录界面
    UFUNCTION(BlueprintCallable, Category = "Login|Flow")
    void ShowLoginUI(APlayerController* PlayerController);
    
    // 用户手动登录（UI调用）
    UFUNCTION(BlueprintCallable, Category = "Login|Flow")
    void ManualLogin(const FString& Username, const FString& Password, APlayerController* PlayerController);
    
    // 登录成功回调（NetworkClientSubsystem调用）
    UFUNCTION(BlueprintCallable, Category = "Login|Flow")
    void OnLoginSuccess(const FString& Token, const FString& UserId, const FString& Username, APlayerController* PlayerController);
    
    // 登录失败回调（NetworkClientSubsystem调用）
    UFUNCTION(BlueprintCallable, Category = "Login|Flow")
    void OnLoginFailed(const FString& ErrorMessage, APlayerController* PlayerController);
    
    // 登出
    UFUNCTION(BlueprintCallable, Category = "Login|Flow")
    void Logout();
    
    // ========== 状态查询 ==========
    
    // 获取当前登录状态
    UFUNCTION(BlueprintCallable, Category = "Login|Status")
    ELoginState GetLoginState() const { return CurrentLoginState; }
    
    // 检查是否已登录
    UFUNCTION(BlueprintCallable, Category = "Login|Status")
    bool IsLoggedIn() const;
    
    // 获取当前用户信息
    UFUNCTION(BlueprintCallable, Category = "Login|Status")
    FString GetCurrentUsername() const;
    
    UFUNCTION(BlueprintCallable, Category = "Login|Status")
    FString GetCurrentUserId() const;
    
    // ========== 事件委托 ==========
    
    UPROPERTY(BlueprintAssignable, Category = "Login|Events")
    FOnLoginStateChanged OnLoginStateChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Login|Events")
    FOnLoginSuccess OnLoginSuccessDelegate;
    
    UPROPERTY(BlueprintAssignable, Category = "Login|Events")
    FOnLoginFailed OnLoginFailedDelegate;

private:
    // ========== 私有方法 ==========
    
    // 设置登录状态
    void SetLoginState(ELoginState NewState);
    
    // 切换到主菜单
    void SwitchToMainMenu(APlayerController* PlayerController);
    
    // 显示登录加载界面
    void ShowLoginLoading(APlayerController* PlayerController, const FString& Message = TEXT("登录中..."));
    
    // 隐藏登录界面
    void HideLoginUI(APlayerController* PlayerController);
    
    // 获取子系统引用
    class UConfigSubsystem* GetConfigSubsystem() const;
    class UNetworkClientSubsystem* GetNetworkSubsystem() const;
    class UUISubsystem* GetUISubsystem() const;
    
    // ========== 成员变量 ==========
    
    // 当前登录状态
    ELoginState CurrentLoginState = ELoginState::None;
    
    // 当前玩家控制器引用
    UPROPERTY()
    APlayerController* CurrentPlayerController = nullptr;
    
    // 是否正在处理登录
    bool bIsProcessingLogin = false;

private:
	// ========== 接受登录结果回调 ==========


    /**delegate5.定义接收信息的委托方法 */
    UFUNCTION()
	void OnStartMediaFinished();
};
