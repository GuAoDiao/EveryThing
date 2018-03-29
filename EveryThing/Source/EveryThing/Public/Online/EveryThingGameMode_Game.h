// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "EveryThingTypes.h"

#include "EveryThingGameMode_Game.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API AEveryThingGameMode_Game : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AEveryThingGameMode_Game();

	//////////////////////////////////////////////////////////////////////////
	/// Game Flow Path
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC);
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	virtual void HandlePlayerLogin(APlayerController* NewPlayer);
	virtual void HandleETGameReay();
	virtual void HandleETGameStart();
	virtual bool HandleETGameIsOver();
	virtual void HandleETGameOver();

	
	//////////////////////////////////////////////////////////////////////////
	/// Damage
	virtual float GetActualDamage(float InDamage, EElementType InDamageType, EElementType PawnElementType, float ElementResistance) const;
	virtual float UpdateEnergy(float energy, float value) { return energy + value; }
	float GetDamageScaleFromElementType(EElementType CauserElementType, EElementType AcceptElementType) const;



	virtual void BeginDestroy() override;
private:
	float NormalDamageScale;
	float SpecificToDamageScale;
};
