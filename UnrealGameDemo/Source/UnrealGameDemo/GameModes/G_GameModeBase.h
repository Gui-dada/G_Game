// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Character/G_PawnData.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"
#include "G_GameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class UNREALGAMEDEMO_API AG_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	AG_GameModeBase();

	UFUNCTION(BlueprintCallable, Category = "G_Game|Pawn")
	const UG_PawnData* GetPawnDataForController(const AController* InController) const;


	/// ~ AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)override;

protected:
	void HanlePlayerLoginInfo();
	bool TryDedicatedServerLogin(AController* NewPlayer, const FUniqueNetIdRepl& UniqueId, const FString& Options);

	void OnSpringLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, AController* NewPlayer);
	
};
