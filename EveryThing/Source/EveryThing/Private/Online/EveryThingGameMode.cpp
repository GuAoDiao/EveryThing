// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameMode.h"


#include "Characters/GamePawn.h"
#include "Characters/Controlllers/PlayerPawnController.h"
#include "EveryThingGameSession.h"

AEveryThingGameMode::AEveryThingGameMode()
{
	NormalDamageScale = 1.f;
	SpecificToDamageScale = 1.5f;

	DefaultPawnClass = AGamePawn::StaticClass();
	PlayerControllerClass = APlayerPawnController::StaticClass();
	GameSessionClass = AEveryThingGameSession::StaticClass();
}

float AEveryThingGameMode::GetActualDamage(float InDamage, EElementType InDamageType, EElementType PawnElementType, float ElementResistance) const
{
	return InDamage * GetDamageScaleFromElementType(InDamageType, PawnElementType);
}

float AEveryThingGameMode::GetDamageScaleFromElementType(EElementType CauserElementType, EElementType AcceptElementType) const
{
	switch (CauserElementType)
	{
		case EElementType::Fire:
			if (AcceptElementType == EElementType::Water) { return SpecificToDamageScale; }
			break;
		case EElementType::Water:
			if (AcceptElementType == EElementType::Fire) { return SpecificToDamageScale; }
			break;
	}

	return NormalDamageScale;
}


