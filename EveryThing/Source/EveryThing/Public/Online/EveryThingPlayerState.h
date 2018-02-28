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

	const FPlayerInfo& GetPlayerInfo() const { return CurrentPlayerInfo; }
private:
	UPROPERTY(Transient, Replicated)
	FPlayerInfo CurrentPlayerInfo;
};
