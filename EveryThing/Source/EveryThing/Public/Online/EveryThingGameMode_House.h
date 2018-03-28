// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EveryThingGameMode_House.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API AEveryThingGameMode_House : public AGameModeBase
{
	GENERATED_BODY()
		
public:
	AEveryThingGameMode_House();

	void OpenGameFromHouseOwner();
	
	void UpdateSessionSetting();

	virtual void BeginPlay() override;
};
