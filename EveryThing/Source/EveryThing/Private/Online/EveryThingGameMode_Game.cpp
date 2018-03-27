// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameMode_Game.h"

#include "Online/PlayerController_Game.h"
#include "Online/EveryThingPlayerState.h"
#include "Online/EveryThingGameState.h"
#include "UI/EveryThingHUD_Game.h"

AEveryThingGameMode_Game::AEveryThingGameMode_Game()
{
	NormalDamageScale = 1.f;
	SpecificToDamageScale = 1.5f;

	DefaultPawnClass = nullptr;
	PlayerControllerClass = APlayerController_Game::StaticClass();
	PlayerStateClass = AEveryThingPlayerState::StaticClass();
	GameStateClass = AEveryThingGameState::StaticClass();
	HUDClass = AEveryThingHUD_Game::StaticClass();
}



float AEveryThingGameMode_Game::GetActualDamage(float InDamage, EElementType InDamageType, EElementType PawnElementType, float ElementResistance) const
{
	return InDamage * GetDamageScaleFromElementType(InDamageType, PawnElementType);
}

float AEveryThingGameMode_Game::GetDamageScaleFromElementType(EElementType CauserElementType, EElementType AcceptElementType) const
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


