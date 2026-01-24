// ConfigSubsystem.cpp
#include "ConfigSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Paths.h"

const FString UConfigSubsystem::SaveSlotName = TEXT("LoginData");
const int32 UConfigSubsystem::SaveUserIndex = 0;

void UConfigSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // 初始化时加载保存数据
    LoadOrCreateSaveGame();

    UE_LOG(LogTemp, Log, TEXT("[ConfigSubsystem] 初始化完成"));
}

// ========== 登录数据操作 ==========

bool UConfigSubsystem::SaveLoginData(const FString& Token, const FString& UserId,
    const FString& Username, int32 ExpireDays, bool bRemember)
{
    if (!CurrentSaveGame)
    {
        CreateDefaultSaveGame();
    }

    if (CurrentSaveGame)
    {
        // 更新登录数据
        CurrentSaveGame->LoginData.AuthToken = Token;
        CurrentSaveGame->LoginData.UserId = UserId;
        CurrentSaveGame->LoginData.Username = Username;
        CurrentSaveGame->LoginData.bRememberMe = bRemember;

        // 设置Token过期时间
        if (ExpireDays > 0)
        {
            CurrentSaveGame->LoginData.TokenExpireTime = FDateTime::Now() + FTimespan::FromDays(ExpireDays);
        }
        else
        {
            CurrentSaveGame->LoginData.TokenExpireTime = FDateTime::Now() + FTimespan::FromHours(1);
        }

        // 保存到磁盘
        bool bSaved = SaveToDisk();
        if (bSaved)
        {
            UE_LOG(LogTemp, Log, TEXT("[ConfigSubsystem] 登录数据保存成功: %s"), *Username);
        }

        return bSaved;
    }

    return false;
}

FLoginData UConfigSubsystem::LoadLoginData()
{
    if (!CurrentSaveGame)
    {
        LoadOrCreateSaveGame();
    }

    if (CurrentSaveGame)
    {
        return CurrentSaveGame->LoginData;
    }

    return FLoginData();
}

void UConfigSubsystem::ClearLoginData()
{
    if (!CurrentSaveGame)
    {
        LoadOrCreateSaveGame();
    }

    if (CurrentSaveGame)
    {
        // 清除敏感数据，保留用户名
        FString OldUsername = CurrentSaveGame->LoginData.Username;

        CurrentSaveGame->LoginData = FLoginData();
        CurrentSaveGame->LoginData.Username = OldUsername; // 保留用户名方便下次登录

        SaveToDisk();

        UE_LOG(LogTemp, Log, TEXT("[ConfigSubsystem] 登录数据已清除"));
    }
}

bool UConfigSubsystem::HasValidLoginData() const
{
    if (!CurrentSaveGame) return false;
    return CurrentSaveGame->LoginData.IsTokenValid();
}

FString UConfigSubsystem::GetCurrentToken() const
{
    if (!CurrentSaveGame) return TEXT("");
    return CurrentSaveGame->LoginData.IsTokenValid() ? CurrentSaveGame->LoginData.AuthToken : TEXT("");
}

FString UConfigSubsystem::GetCurrentUserId() const
{
    if (!CurrentSaveGame) return TEXT("");
    return CurrentSaveGame->LoginData.UserId;
}

FString UConfigSubsystem::GetCurrentUsername() const
{
    if (!CurrentSaveGame) return TEXT("");
    return CurrentSaveGame->LoginData.Username;
}

// ========== 工具方法 ==========

bool UConfigSubsystem::SaveToDisk()
{
    if (!CurrentSaveGame) return false;

    bool bSuccess = UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, SaveUserIndex);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("[ConfigSubsystem] 数据保存到磁盘"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[ConfigSubsystem] 数据保存失败"));
    }

    return bSuccess;
}

void UConfigSubsystem::ReloadFromDisk()
{
    LoadOrCreateSaveGame();
}

bool UConfigSubsystem::DeleteSaveFile()
{
    bool bSuccess = UGameplayStatics::DeleteGameInSlot(SaveSlotName, SaveUserIndex);

    if (bSuccess)
    {
        CurrentSaveGame = nullptr;
        CreateDefaultSaveGame();
        UE_LOG(LogTemp, Log, TEXT("[ConfigSubsystem] 保存文件已删除"));
    }

    return bSuccess;
}

// ========== 私有方法 ==========

void UConfigSubsystem::LoadOrCreateSaveGame()
{
    // 尝试加载现有存档
    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
    {
        CurrentSaveGame = Cast<ULoginSaveGame>(
            UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex)
        );

        if (CurrentSaveGame)
        {
            UE_LOG(LogTemp, Log, TEXT("[ConfigSubsystem] 登录数据加载成功"));

            // 版本迁移检查
            if (CurrentSaveGame->SaveVersion < 1)
            {
                CurrentSaveGame->SaveVersion = 1;
                SaveToDisk();
            }
        }
    }

    // 如果没有存档，创建默认的
    if (!CurrentSaveGame)
    {
        CreateDefaultSaveGame();
    }
}

void UConfigSubsystem::CreateDefaultSaveGame()
{
    CurrentSaveGame = Cast<ULoginSaveGame>(
        UGameplayStatics::CreateSaveGameObject(ULoginSaveGame::StaticClass())
    );

    if (CurrentSaveGame)
    {
        CurrentSaveGame->SaveVersion = 1;
        CurrentSaveGame->LoginData = FLoginData();

        UE_LOG(LogTemp, Log, TEXT("[ConfigSubsystem] 创建默认登录数据"));
    }
}

void UConfigSubsystem::ApplyAutoLoginSettings()
{
    // 如果需要自动登录，可以在这里设置
}