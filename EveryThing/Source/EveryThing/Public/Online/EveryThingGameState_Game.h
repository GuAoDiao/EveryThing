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

	float GetRemaningReadyTime() const { return RemaningReadyTime; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemaningReadyTimeUpdateDelegate, float /*RemaningReadyTime*/)
	FOnRemaningReadyTimeUpdateDelegate OnRemaningReadyTimeUpdateDelegate;
protected:
	FTimerHandle ReadyCountDownTimer;
	float DefaultReadyTime;

	UFUNCTION()
	void OnRep_RemaningReadyTime() { OnRemaningReadyTimeUpdate(); }
	void OnRemaningReadyTimeUpdate() { OnRemaningReadyTimeUpdateDelegate.Broadcast(RemaningReadyTime); }

	UPROPERTY(Transient, ReplicatedUsing = OnRep_RemaningReadyTime)
	float RemaningReadyTime;

	/// Gameing
public:
	void StartGameTimeCountDown();
	void GameTimeCountDown();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemaningGameTimeUpdateDelegate, float /* RemaningGameTime */)
	FOnRemaningGameTimeUpdateDelegate OnRemaningGameTimeUpdateDelegate;
	UFUNCTION()
	void OnRep_RemaningGameTime() { OnRemaningGameTimeUpdate(); }
	void OnRemaningGameTimeUpdate() { OnRemaningGameTimeUpdateDelegate.Broadcast(RemaningGameTime); }

	float GetRemaningGameTime() const { return RemaningGameTime; }

	FTimerHandle GameTimeCountDownTimer;
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RemaningGameTime)
	float RemaningGameTime;
	float DefaultGameTime;

	/// Back to house
public:
	void StartBackToHouseCountDown();
	void BackToHouseeCountDown();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemaningBackToHouseTimeUpdateDelegate, float /* RemaningGameTime */)
	FOnRemaningBackToHouseTimeUpdateDelegate OnRemaningBackToHouseTimeUpdateDelegate;
	UFUNCTION()
	void OnRep_RemaningBackToHouseTime() { OnRemaningBackToHouseTimeUpdate(); }
	void OnRemaningBackToHouseTimeUpdate() { OnRemaningBackToHouseTimeUpdateDelegate.Broadcast(RemaningBackToHouseTime); }

	float GetRemaningBackToHouseTime() const { return RemaningBackToHouseTime; }

	FTimerHandle BackToHouseTimeCountDownTimer;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RemaningGameTime)
	float RemaningBackToHouseTime;
	float DefaultBackToHouseTime;

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
	/// For Chat Window Game State Interface
public:
	virtual const FString& GetPlayerChatName(int32 PlayerID) const override;
	virtual const TArray<APlayerState*>& GetAllChatPlayerState() const override { return ChatPlayerState; }
	const static FString PlayerChatName_NONE;
	int32 GetNextChatID(APlayerState* PlayerState) { return ChatPlayerState.Add(PlayerState);}
	UPROPERTY(Transient, Replicated)
	TArray<APlayerState*> ChatPlayerState;
};
