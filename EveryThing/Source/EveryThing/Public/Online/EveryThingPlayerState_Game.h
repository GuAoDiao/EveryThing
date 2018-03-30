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
	/// For Chat Window Player State Interface
public:
	virtual const FString& GetPlayerChatName() const override { return CurrentPlayerInfo.PlayerName; }
	virtual int32 GetPlayerChatID() const override { return ChatID; }
	virtual int32 GetPlayerChatTeamID() const override { return TeamID; }

	UPROPERTY(Transient, Replicated)
	int32 ChatID;
};
