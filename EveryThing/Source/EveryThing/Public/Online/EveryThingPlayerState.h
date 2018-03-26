// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "ChatWindow/ChatWindowPlayerStateInterface.h"
#include "EveryThingTypes.h"

#include "EveryThingPlayerState.generated.h"


class AGamePawn;
class APlayerPawnController;
/**
 * 
 */
UCLASS()
class EVERYTHING_API AEveryThingPlayerState : public APlayerState, public IChatWindowPlayerStateInterface
{
	GENERATED_BODY()
	
public:
	virtual void OnConstruction(const FTransform& Transform) override;

	//////////////////////////////////////////////////////////////////////////
	/// PlayerInfo
public:
	const FPlayerInfo& GetPlayerInfo() const { return CurrentPlayerInfo; }
	void SetPlayerInfo(const FPlayerInfo& InPlayerInfo);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerInfo(const FPlayerInfo& InPlayerInfo);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdatePlayerInfoDelegate, const FPlayerInfo&);
	FOnUpdatePlayerInfoDelegate OnUpdatePlayerInfoDelegate;

private:
	UFUNCTION()
	void OnRep_CurrentPlayerInfo();
	void OnPlayerInfoUpdate();

	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentPlayerInfo)
	FPlayerInfo CurrentPlayerInfo;

	//////////////////////////////////////////////////////////////////////////
	/// For Chat Window Player State Interface
public:
	virtual const FString& GetPlayerChatName() const { return CurrentPlayerInfo.PlayerName; }
	virtual int32 GetPlayerlChatID() const { return ChatID; }
	void SetPlayerChatID(int32 InChatID) { if (HasAuthority()) { ChatID = InChatID; } }

	UPROPERTY(Transient, Replicated)
	int32 ChatID;
};
