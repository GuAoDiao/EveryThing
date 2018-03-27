// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingPlayerState_Game.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"

#include "EveryThingGameInstance.h"

#include "Characters/GamePawn.h"
#include "Characters/PlayerPawns/PlayerChairPawn.h"
#include "Characters/PlayerPawns/PlayerFootballPawn.h"

void AEveryThingPlayerState_Game::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	UEveryThingGameInstance* OwnerETGI = GetWorld() ? Cast<UEveryThingGameInstance>(GetWorld()->GetGameInstance()) : nullptr;
	if (OwnerETGI) { SetPlayerInfo( OwnerETGI->GetPlayerInfo()); }
}

void AEveryThingPlayerState_Game::SetPlayerInfo(const FPlayerInfo& InPlayerInfo)
{
	if (!HasAuthority())
	{ 
		ServerSetPlayerInfo(InPlayerInfo);
	}

	CurrentPlayerInfo = InPlayerInfo;
	
	OnPlayerInfoUpdate();
}


bool AEveryThingPlayerState_Game::ServerSetPlayerInfo_Validate(const FPlayerInfo& InPlayerInfo) { return true; }
void AEveryThingPlayerState_Game::ServerSetPlayerInfo_Implementation(const FPlayerInfo& InPlayerInfo) { SetPlayerInfo(InPlayerInfo); }

void AEveryThingPlayerState_Game::OnPlayerInfoUpdate() { OnUpdatePlayerInfoDelegate.Broadcast(CurrentPlayerInfo); }
void AEveryThingPlayerState_Game::OnRep_CurrentPlayerInfo() { OnPlayerInfoUpdate(); }

void AEveryThingPlayerState_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingPlayerState_Game, CurrentPlayerInfo);
	DOREPLIFETIME(AEveryThingPlayerState_Game, ChatID);
}