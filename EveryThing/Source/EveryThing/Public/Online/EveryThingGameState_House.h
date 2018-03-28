// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "EveryThingGameState_House.generated.h"

class AEveryThingPlayerState_House;


/**
 * 
 */
UCLASS()
class EVERYTHING_API AEveryThingGameState_House : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void AddPlayerState(APlayerState* PlayerState) override;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddPlayerDelegate, APlayerState* /* PlayerState */);
	FOnAddPlayerDelegate OnAddPlayerDelegate;


	bool CheckPlayerIsHouseOwner(AEveryThingPlayerState_House* InPlayer) const { return InPlayer == HouseOwner; }
	bool CheckIsAllPlayerAreReady();

	void SetPlayerHouseOwner(AEveryThingPlayerState_House* InPlayer) { if (HasAuthority()) { HouseOwner = InPlayer; } }

	UPROPERTY(Transient, Replicated)
	FString GameType;
	UPROPERTY(Transient, Replicated)
	FString MapName;
	UPROPERTY(Transient, Replicated)
	FString HouseName;

	UPROPERTY(Transient, Replicated)
	bool bIsLANMatch;
	UPROPERTY(Transient, Replicated)
	int32 MaxPlayerNum;

	UPROPERTY(Transient, Replicated)
	FString CurrentHouseName;

	UPROPERTY(Transient, Replicated)
	AEveryThingPlayerState_House* HouseOwner;
};
