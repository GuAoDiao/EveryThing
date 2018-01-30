// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
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

	void ToggolePawn(int32 NumberIndex);

private:
	TArray<TSubclassOf<AGamePawn>> AllGamePawn;

	TSubclassOf<AGamePawn> CurrentPawnClass;
};
