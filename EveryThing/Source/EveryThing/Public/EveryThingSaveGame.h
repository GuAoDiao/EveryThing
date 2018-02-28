// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "EveryThingTypes.h"

#include "EveryThingSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UEveryThingSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	void InitializeDefaultPlayerInfo(const FString& PlayerName);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPlayerInfo PlayerInfo;	
};