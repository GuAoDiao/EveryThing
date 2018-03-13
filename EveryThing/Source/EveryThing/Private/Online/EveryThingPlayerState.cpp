// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingPlayerState.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"

#include "EveryThingGameInstance.h"

#include "Characters/GamePawn.h"
#include "Characters/PlayerPawns/PlayerChairPawn.h"
#include "Characters/PlayerPawns/PlayerFootballPawn.h"

void AEveryThingPlayerState::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	UEveryThingGameInstance* OwnerETGI = GetWorld() ? Cast<UEveryThingGameInstance>(GetWorld()->GetGameInstance()) : nullptr;
	if (OwnerETGI) { SetPlayerInfo( OwnerETGI->GetPlayerInfo()); }
}

void AEveryThingPlayerState::SetPlayerInfo(const FPlayerInfo& InPlayerInfo)
{
	if (!HasAuthority())
	{ 
		ServerSetPlayerInfo(InPlayerInfo);
	}

	CurrentPlayerInfo = InPlayerInfo;
	
	OnPlayerInfoUpdate();
}


bool AEveryThingPlayerState::ServerSetPlayerInfo_Validate(const FPlayerInfo& InPlayerInfo) { return true; }
void AEveryThingPlayerState::ServerSetPlayerInfo_Implementation(const FPlayerInfo& InPlayerInfo) { SetPlayerInfo(InPlayerInfo); }

void AEveryThingPlayerState::OnPlayerInfoUpdate() { OnUpdatePlayerInfoDelegate.Broadcast(CurrentPlayerInfo); }
void AEveryThingPlayerState::OnRep_CurrentPlayerInfo() { OnPlayerInfoUpdate(); }

void AEveryThingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingPlayerState, CurrentPlayerInfo);
	DOREPLIFETIME(AEveryThingPlayerState, ChatID);
}