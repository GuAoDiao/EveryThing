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
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddPlayerDelegate, APlayerState* /* PlayerState */);
	FOnAddPlayerDelegate OnAddPlayerDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemovePlayerDelegate, APlayerState* /* PlayerState */);
	FOnRemovePlayerDelegate OnRemovePlayerDelegate;

	bool CheckPlayerIsHouseOwner(AEveryThingPlayerState_House* InPlayer) const { return InPlayer == HouseOwner; }
	bool CheckIsAllPlayerAreReady();

	void SetPlayerHouseOwner(AEveryThingPlayerState_House* InPlayer) { if (HasAuthority()) { HouseOwner = InPlayer; } }
	

	DECLARE_MULTICAST_DELEGATE_FiveParams(FOnHouseSettingUpdateDelegate, const FString& /*HouseName*/, const FString& /*GameType*/, const FString& /*MapName*/, bool /*bIsLAN*/, int32 /*MaxPlayersNum*/);
	FOnHouseSettingUpdateDelegate OnHouseSettingUpdateDelegate;
	UFUNCTION()
	void OnHouseSettingUpdate() { OnHouseSettingUpdateDelegate.Broadcast(HouseName, GameType, MapName, bIsLANMatch, MaxPlayerNum); }

	UPROPERTY(Transient, ReplicatedUsing = OnHouseSettingUpdate)
	FString GameType;
	UPROPERTY(Transient, ReplicatedUsing = OnHouseSettingUpdate)
	FString MapName;
	UPROPERTY(Transient, ReplicatedUsing = OnHouseSettingUpdate)
	FString HouseName;
	UPROPERTY(Transient, ReplicatedUsing = OnHouseSettingUpdate)
	bool bIsLANMatch;
	UPROPERTY(Transient, ReplicatedUsing = OnHouseSettingUpdate)
	int32 MaxPlayerNum;

	UPROPERTY(Transient, Replicated)
	FString CurrentHouseName;

	UPROPERTY(Transient, Replicated)
	AEveryThingPlayerState_House* HouseOwner;
};
