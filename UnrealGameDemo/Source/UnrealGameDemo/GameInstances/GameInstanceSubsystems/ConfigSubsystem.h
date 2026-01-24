// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameFramework/SaveGame.h"
#include "ConfigSubsystem.generated.h"

///////////////////ConfigTypes/////////////////////
// 登录数据（仅存储必要信息）
USTRUCT(BlueprintType)
struct FLoginData
{
    GENERATED_BODY()
    
    // 登录令牌
    UPROPERTY(BlueprintReadWrite, Category = "LocalPlayerInfo")
    FString AuthToken;
    
    // 用户ID
    UPROPERTY(BlueprintReadWrite, Category = "LocalPlayerInfo")
    FString UserId;
    
    // 用户名（显示用）
    UPROPERTY(BlueprintReadWrite, Category = "LocalPlayerInfo")
    FString Username;
    
    // Token过期时间
    UPROPERTY(BlueprintReadWrite, Category = "LocalPlayerInfo")
    FDateTime TokenExpireTime;
    
    // 是否记住登录状态
    UPROPERTY(BlueprintReadWrite, Category = "LocalPlayerInfo")
    bool bRememberMe = false;
    
    FLoginData()
    {
        AuthToken = TEXT("");
        UserId = TEXT("");
        Username = TEXT("Guest");
        TokenExpireTime = FDateTime::MinValue();
        bRememberMe = false;
    }
    
    // 检查Token是否有效
    bool IsTokenValid() const
    {
        if (AuthToken.IsEmpty()) return false;
        FDateTime Now = FDateTime::Now();
        return Now < TokenExpireTime;
    }
};

// 保存游戏数据
UCLASS()
class ULoginSaveGame : public USaveGame
{
    GENERATED_BODY()
    
public:
    UPROPERTY()
    FLoginData LoginData;
    
    UPROPERTY()
    int32 SaveVersion = 1;
};

///////////////////////////////////////////////////////////////////


/**
 * 
 */
UCLASS()
class UNREALGAMEDEMO_API UConfigSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// ========== 生命周期 ==========
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// ========== 登录数据操作 ==========
	// 保存登录数据
	UFUNCTION(BlueprintCallable, Category = "Config|Login")
	bool SaveLoginData(const FString& Token, const FString& UserId, const FString& Username,
		int32 ExpireDays = 7, bool bRemember = false);

	// 加载登录数据
	UFUNCTION(BlueprintCallable, Category = "Config|Login")
	FLoginData LoadLoginData();

	// 清除登录数据（登出时使用）
	UFUNCTION(BlueprintCallable, Category = "Config|Login")
	void ClearLoginData();

	// 检查是否有有效登录数据
	UFUNCTION(BlueprintCallable, Category = "Config|Login")
	bool HasValidLoginData() const;

	// 获取当前Token（如果有效）
	UFUNCTION(BlueprintCallable, Category = "Config|Login")
	FString GetCurrentToken() const;

	// 获取当前用户ID
	UFUNCTION(BlueprintCallable, Category = "Config|Login")
	FString GetCurrentUserId() const;

	// 获取当前用户名
	UFUNCTION(BlueprintCallable, Category = "Config|Login")
	FString GetCurrentUsername() const;

	// ========== 工具方法 ==========
	// 强制保存到磁盘
	UFUNCTION(BlueprintCallable, Category = "Config")
	bool SaveToDisk();

	// 重新加载数据
	UFUNCTION(BlueprintCallable, Category = "Config")
	void ReloadFromDisk();

	// 删除保存文件
	UFUNCTION(BlueprintCallable, Category = "Config")
	bool DeleteSaveFile();

private:
	// 当前保存游戏对象
	UPROPERTY()
	ULoginSaveGame* CurrentSaveGame;

	// 存档配置
	static const FString SaveSlotName;
	static const int32 SaveUserIndex;

	// 内部方法
	void LoadOrCreateSaveGame();
	void CreateDefaultSaveGame();
	void ApplyAutoLoginSettings();
	
};
