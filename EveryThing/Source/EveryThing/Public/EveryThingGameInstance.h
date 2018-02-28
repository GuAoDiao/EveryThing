// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EveryThingTypes.h"
#include "EveryThingGameInstance.generated.h"

class UEveryThingSaveGame;

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
	void OpenMenuLevel();
	void OpenGameLevel(const FString& MapType, const FString& MapName);
	void ExitGameApplication();

	//////////////////////////////////////////////////////////////////////////
	/// Match
public:
	void HostGame(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum);
	void FindHoustList(bool bIsLAN, bool bIsPresence);
	void JoinGame(FOnlineSessionSearchResult& SessionResult);
	void JoinGame(int32 SearchResultIndex);
	void ExitGame();

private:
	class AEveryThingGameSession* GetGameSession();
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName MenuLevelName;

	//////////////////////////////////////////////////////////////////////////
	/// Player Info
public:
	bool LoadPlayerInfoFromSlotName(const FString& SlotName);
	bool SavePlayerInfoWithSlotName(const FString& SlotName);
	bool SavePlayerInfoToCurrentSlotName();

	void SetPlayerInfo(const FPlayerInfo& InPlayerInfo) { PlayerInfo = InPlayerInfo; }
	const FPlayerInfo& GetPlayerInfo() const { return PlayerInfo; }
private:
	FPlayerInfo PlayerInfo;
	FString SaveDataSlotName;
};
