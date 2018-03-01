// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EveryThingTypes.h"
#include "EveryThingPlayerState.generated.h"


class AGamePawn;
class APlayerPawnController;
/**
 * 
 */
UCLASS()
class EVERYTHING_API AEveryThingPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AEveryThingPlayerState();

	virtual void BeginPlay() override;

	//////////////////////////////////////////////////////////////////////////
	/// PlayerInfo
public:
	const FPlayerInfo& GetPlayerInfo() const { return CurrentPlayerInfo; }
	void SetPlayerInfo(const FPlayerInfo& InPlayerInfo);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdatePlayerInfoDelegate, const FPlayerInfo&);
	FOnUpdatePlayerInfoDelegate GetOnUpdatePlayerInfoDelegate() { return OnUpdatePlayerInfoDelegate; }

private:
	UFUNCTION()
	void OnRep_CurrentPlayerInfo();
	void OnPlayerInfoUpdate();

	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentPlayerInfo)
	FPlayerInfo CurrentPlayerInfo;
	FOnUpdatePlayerInfoDelegate OnUpdatePlayerInfoDelegate;
};
