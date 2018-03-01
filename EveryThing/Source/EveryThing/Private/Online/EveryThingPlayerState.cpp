// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingPlayerState.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"

#include "EveryThingGameInstance.h"

#include "Characters/GamePawn.h"
#include "Characters/PlayerPawns/PlayerChairPawn.h"
#include "Characters/PlayerPawns/PlayerFootballPawn.h"

AEveryThingPlayerState::AEveryThingPlayerState()
{
}

void AEveryThingPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	UEveryThingGameInstance* OwnerETGI = GetWorld() ? Cast<UEveryThingGameInstance>(GetWorld()->GetGameInstance()) : nullptr;
	if (OwnerETGI) { SetPlayerInfo( OwnerETGI->GetPlayerInfo()); }
}

void AEveryThingPlayerState::SetPlayerInfo(const FPlayerInfo& InPlayerInfo)
{
	CurrentPlayerInfo = InPlayerInfo;

	CurrentPlayerInfo.AllHaveRolesName.Add("Football");
	CurrentPlayerInfo.AllHaveRolesName.Add("Chair");

	OnPlayerInfoUpdate();
}

void AEveryThingPlayerState::OnPlayerInfoUpdate()
{
	OnUpdatePlayerInfoDelegate.Broadcast(CurrentPlayerInfo);
}

void AEveryThingPlayerState::OnRep_CurrentPlayerInfo()
{
	OnPlayerInfoUpdate();
}

void AEveryThingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingPlayerState, CurrentPlayerInfo);
}