// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "EveryThingTypes.h"
#include "ChatWIndow/ChatWindowGameStateInterface.h"

#include "EveryThingGameState_Game.generated.h"


class AEveryThingPlayerState_Game;

/**
 * 
 */
UCLASS()
class EVERYTHING_API AEveryThingGameState_Game : public AGameStateBase, public IChatWindowGameStateInterface
{
	GENERATED_BODY()
	
public:
	AEveryThingGameState_Game();


	//////////////////////////////////////////////////////////////////////////
	/// Game flow path
public:
	void InitializeETGameState(const FString& InGameType, const FString& InMapName, const FString& InHouseName, bool bInIsLANMatach, int32 InMaxPlayerNum, int32 InCurrentPlayerNum);
	
	void OnPlayerPostLogin(APlayerController* NewPlayer);
	void OnPlayerLogout(AController* Exiting);
	/// Ready
	void StartReadyCountDown();
	void ReadyCountDown();

	float GetReadyRemaningTime() const { return ReadyRemaningTime; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnReadyRemaningTimeUpdateDelegate, float /*ReadyRemaningTime*/)
	FOnReadyRemaningTimeUpdateDelegate OnReadyRemaningTimeUpdateDelegate;
protected:
	FTimerHandle ReadyCountDownTimer;
	float DefaultReadyTime;

	UFUNCTION()
	void OnRep_ReadyRemaningTime() { OnReadyRemaningTimeUpdate(); }
	void OnReadyRemaningTimeUpdate() { OnReadyRemaningTimeUpdateDelegate.Broadcast(ReadyRemaningTime); }

	UPROPERTY(Transient, ReplicatedUsing = OnRep_ReadyRemaningTime)
	float ReadyRemaningTime;

	//////////////////////////////////////////////////////////////////////////
	/// Game State Attr
public:
	UPROPERTY(Transient, Replicated)
	FString GameType;
	UPROPERTY(Transient, Replicated)
	FString MapName;
	UPROPERTY(Transient, Replicated)
	FString HouseName;
	UPROPERTY(Transient, Replicated)
	bool bIsLANMatach;
	UPROPERTY(Transient, Replicated)
	int32 MaxPlayerNum;
	UPROPERTY(Transient, Replicated)
	int32 CurrentPlayerNum;

	int32 HousePlayerNum;

	//////////////////////////////////////////////////////////////////////////
	/// Game State
public:
	bool IsTargetETGameState(EETGameState TargetGameStae) { return CurrentETGameState == TargetGameStae; }
	EETGameState GetETGameState() const {return CurrentETGameState;}
	void ToggleToTargetETGameState(EETGameState TargetGameStae);

	bool IsETGameStarted() const {return bIsETGameStarted;}
protected:
	UPROPERTY(Transient, Replicated)
	EETGameState CurrentETGameState;
	UPROPERTY(Transient, Replicated)
	bool bIsETGameStarted;
	//////////////////////////////////////////////////////////////////////////
	/// Player Add And Leave
public:
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	//////////////////////////////////////////////////////////////////////////
	/// For Chat Window Game State Interface
public:
	virtual const FString& GetPlayerChatName(int32 PlayerID) const override;
	virtual const TArray<APlayerState*>& GetAllChatPlayerState() const override { return PlayerArray; }
	const static FString PlayerChatName_NONE;
};
