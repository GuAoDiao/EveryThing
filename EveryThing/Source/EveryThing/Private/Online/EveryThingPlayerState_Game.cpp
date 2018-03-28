// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingPlayerState_Game.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"

#include "EveryThingGameInstance.h"
#include "Online/EveryThingPlayerState_House.h"

#include "Characters/GamePawn.h"
#include "Characters/PlayerPawns/PlayerChairPawn.h"
#include "Characters/PlayerPawns/PlayerFootballPawn.h"

void AEveryThingPlayerState_Game::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner() && GetOwner()->Role == ROLE_AutonomousProxy && !bFromPreviousLevel)
	{
		UEveryThingGameInstance* OwnerETGI = Cast<UEveryThingGameInstance>(GetGameInstance());
		if (OwnerETGI) { ServerSetPlayerInfo( OwnerETGI->GetPlayerInfo()); }
	}
}

void AEveryThingPlayerState_Game::SetPlayerInfo(const FPlayerInfo& InPlayerInfo)
{
	if (HasAuthority())
	{ 
		CurrentPlayerInfo = InPlayerInfo;
	
		OnPlayerInfoUpdate();
	}
}

void AEveryThingPlayerState_Game::SetTeamID(int32 InTeamID)
{
	if (HasAuthority())
	{
		TeamID = InTeamID;
		OnTeamIDUpdate();
	}
}

bool AEveryThingPlayerState_Game::ServerSetPlayerInfo_Validate(const FPlayerInfo& InPlayerInfo) { return true; }
void AEveryThingPlayerState_Game::ServerSetPlayerInfo_Implementation(const FPlayerInfo& InPlayerInfo) { SetPlayerInfo(InPlayerInfo); }

void AEveryThingPlayerState_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingPlayerState_Game, CurrentPlayerInfo);
	DOREPLIFETIME(AEveryThingPlayerState_Game, ChatID);
	DOREPLIFETIME(AEveryThingPlayerState_Game, TeamID);
}