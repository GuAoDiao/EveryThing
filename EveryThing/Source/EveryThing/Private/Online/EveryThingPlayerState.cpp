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
	UEveryThingGameInstance* OwnerETGI = GetWorld() ? Cast<UEveryThingGameInstance>(GetGameInstance()) : nullptr;
	if (OwnerETGI) { CurrentPlayerInfo = OwnerETGI->GetPlayerInfo(); }


	CurrentPlayerInfo.AllHaveRolesName.Add("Football");
	CurrentPlayerInfo.AllHaveRolesName.Add("Chair");
}


void AEveryThingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingPlayerState, CurrentPlayerInfo);
}