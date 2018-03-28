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
	/// Damage
	virtual float GetActualDamage(float InDamage, EElementType InDamageType, EElementType PawnElementType, float ElementResistance) const;
	virtual float UpdateEnergy(float energy, float value) { return energy + value; }
	float GetDamageScaleFromElementType(EElementType CauserElementType, EElementType AcceptElementType) const;



	virtual void BeginDestroy() override;
private:
	float NormalDamageScale;
	float SpecificToDamageScale;
};
