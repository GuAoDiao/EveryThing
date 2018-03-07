// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameMode.h"

#include "Characters/PlayerPawnController.h"
#include "Online/EveryThingPlayerState.h"
#include "Online/EveryThingGameState.h"
#include "UI/Game/EveryThingGameHUD.h"

AEveryThingGameMode::AEveryThingGameMode()
{
	NormalDamageScale = 1.f;
	SpecificToDamageScale = 1.5f;

	DefaultPawnClass = nullptr;
	PlayerControllerClass = APlayerPawnController::StaticClass();
	PlayerStateClass = AEveryThingPlayerState::StaticClass();
	GameStateClass = AEveryThingGameState::StaticClass();
	HUDClass = AEveryThingGameHUD::StaticClass();
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


