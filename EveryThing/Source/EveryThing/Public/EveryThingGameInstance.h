// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EveryThingGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UEveryThingGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UEveryThingGameInstance();

	
	//////////////////////////////////////////////////////////////////////////
	/// Level
public:
	UFUNCTION(BlueprintCallable)
	void OpenMenuLevel();
	UFUNCTION(BlueprintCallable)
	void OpenGameLevel(const FName& LevelName);


	//////////////////////////////////////////////////////////////////////////
	/// Match
public:
	void HostGame(const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum);
	void ExitGame();
	void FindHoustList();
	void JoinGame(FOnlineSessionSearchResult& SessionResult);
	void JoinGame(FName SessionName, int32 SearchResultIndex);

private:
	class AEveryThingGameSession* GetGameSession();
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName MenuLevelName;
};
