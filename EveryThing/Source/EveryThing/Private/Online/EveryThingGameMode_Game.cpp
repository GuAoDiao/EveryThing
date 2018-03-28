// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameMode_Game.h"

#include "Online/PlayerController_Game.h"
#include "Online/EveryThingPlayerState_Game.h"
#include "Online/EveryThingGameState_Game.h"
#include "Online/EveryThingGameSession.h"
#include "UI/EveryThingHUD_Game.h"

#include "PlayerFootballPawn.h"

AEveryThingGameMode_Game::AEveryThingGameMode_Game()
{
	NormalDamageScale = 1.f;
	SpecificToDamageScale = 1.5f;

	DefaultPawnClass = APlayerFootballPawn::StaticClass();
	
	GameSessionClass = AEveryThingGameSession::StaticClass();
	PlayerControllerClass = APlayerController_Game::StaticClass();
	PlayerStateClass = AEveryThingPlayerState_Game::StaticClass();
	GameStateClass = AEveryThingGameState_Game::StaticClass();
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


void AEveryThingGameMode_Game::BeginDestroy()
{

	AEveryThingGameSession* OwnerGameSession = Cast<AEveryThingGameSession>(GameSession);
	if (OwnerGameSession) { OwnerGameSession->DestroySession(); }

	Super::BeginDestroy();
}