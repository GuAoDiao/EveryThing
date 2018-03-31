// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "ChatWindow/ChatWindowPlayerStateInterface.h"
#include "EveryThingTypes.h"

#include "EveryThingPlayerState_Game.generated.h"


class AGamePawn;
class APlayerController_Game;
/**
 * 
 */
UCLASS()
class EVERYTHING_API AEveryThingPlayerState_Game : public APlayerState, public IChatWindowPlayerStateInterface
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	void SeamlessTravelTo(class APlayerState* NewPlayerState);
	//////////////////////////////////////////////////////////////////////////
	/// PlayerInfo
public:
	const FPlayerInfo& GetPlayerInfo() const { return CurrentPlayerInfo; }
	void SetPlayerInfo(const FPlayerInfo& InPlayerInfo);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerInfo(const FPlayerInfo& InPlayerInfo);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdatePlayerInfoDelegate, const FPlayerInfo&);
	FOnUpdatePlayerInfoDelegate OnPlayerInfoUpdateDelegate;
	
private:
	UFUNCTION()
	void OnRep_CurrentPlayerInfo() { OnPlayerInfoUpdate(); }
	void OnPlayerInfoUpdate() { OnPlayerInfoUpdateDelegate.Broadcast(CurrentPlayerInfo); }

	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentPlayerInfo)
	FPlayerInfo CurrentPlayerInfo;

	//////////////////////////////////////////////////////////////////////////
	/// Team
public:
	int32 GetTeamID() const { return TeamID; }
	void SetTeamID(int32 InTeamID) { TeamID = InTeamID; OnTeamIDUpdate(); }
	void ChangeTeamID(int32 InTeamID);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeamIDUpdateDelegate, int32 /* TeamID */);
	FOnTeamIDUpdateDelegate OnTeamIDUpdateDelegate;

protected:
	UFUNCTION()
	void OnRep_TeamID() { OnTeamIDUpdate(); }
	void OnTeamIDUpdate() { OnTeamIDUpdateDelegate.Broadcast(TeamID); }

	UPROPERTY(Transient, ReplicatedUsing = OnRep_TeamID)
	int32 TeamID;

	//////////////////////////////////////////////////////////////////////////
	/// About Game Fight Info
public:
	int32 GetGameScore() const { return GameScore; }
	int32 GetKillNum() const { return KillNum; }
	int32 GetDeathNum() const { return DeathNum; }

	void AddGameScore(int32 InOffset);
	void IncKillNum() { ++KillNum; OnKillNumUpdate(); }
	void IncDeathNum() { ++DeathNum; OnDeathNumUpdate(); }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameScoreOffsetDelegate, int32 /* GameScoreOffset */);
	FOnGameScoreOffsetDelegate OnGameScoreOffsetDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameScoreUpdateDelegate, int32 /* GameScore */);
	FOnGameScoreUpdateDelegate OnGameScoreUpdateDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnKillNumUpdateDelegate, int32 /* KillNum */);
	FOnKillNumUpdateDelegate OnKillNumUpdateDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeathNumUpdateDelegate, int32 /* DeathNum */);
	FOnDeathNumUpdateDelegate OnDeathNumUpdateDelegate;
protected:
	UFUNCTION()
	void OnRep_GameScore() { OnGameScoreUpdate(); }
	void OnGameScoreUpdate() { OnGameScoreOffsetDelegate.Broadcast(GameScore); }
	UFUNCTION()
	void OnRep_KillNum() {}
	void OnKillNumUpdate() { OnKillNumUpdateDelegate.Broadcast(KillNum); }
	UFUNCTION()
	void OnRep_DeathNum() {}
	void OnDeathNumUpdate() { OnDeathNumUpdateDelegate.Broadcast(DeathNum); }

	UPROPERTY(Transient, Replicated)
	int32 GameScore;
	UPROPERTY(Transient, Replicated)
	int32 KillNum;
	UPROPERTY(Transient, Replicated)
	int32 DeathNum;
	//////////////////////////////////////////////////////////////////////////
	/// For Chat Window Player State Interface
public:
	virtual const FString& GetPlayerChatName() const override { return CurrentPlayerInfo.PlayerName; }
	virtual int32 GetPlayerChatID() const override { return ChatID; }
	virtual int32 GetPlayerChatTeamID() const override { return TeamID; }

	UPROPERTY(Transient, Replicated)
	int32 ChatID;
};
