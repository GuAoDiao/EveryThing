// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController_House.h"

#include "Engine/World.h"

#include "Online/EveryThingPlayerState_House.h"
#include "Online/EveryThingGameState_House.h"
#include "Online/EveryThingGameMode_House.h"




void APlayerController_House::StartGameWhenIsHouseOwner()
{
	AEveryThingPlayerState_House* OwnerETPS_H = Cast<AEveryThingPlayerState_House>(PlayerState);
	
	if (!OwnerETPS_H->CheckIsHouseOwner()) { return; }
	
	if (!HasAuthority())
	{
		ServerStartGameWhenIsHouseOwner();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		AEveryThingGameState_House* OwnerETGS_H = World->GetGameState<AEveryThingGameState_House>();
		if (OwnerETGS_H && OwnerETGS_H->CheckIsAllPlayerAreReady())
		{
			AEveryThingGameMode_House* OwnerETGM_H = World->GetAuthGameMode<AEveryThingGameMode_House>();
			if (OwnerETGM_H) { OwnerETGM_H->OpenGameFromHouseOwner(); }
		}
	}
}

bool APlayerController_House::ServerStartGameWhenIsHouseOwner_Validate() { return true; }
void APlayerController_House::ServerStartGameWhenIsHouseOwner_Implementation() { StartGameWhenIsHouseOwner(); }


void APlayerController_House::ApplyHouseSetting()
{
	AEveryThingPlayerState_House* OwnerETPS_H = Cast<AEveryThingPlayerState_House>(PlayerState);

	if (!OwnerETPS_H->CheckIsHouseOwner()) { return; }

	if (!HasAuthority())
	{
		ServerStartGameWhenIsHouseOwner();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		
		AEveryThingGameMode_House* OwnerETGM_H = World->GetAuthGameMode<AEveryThingGameMode_House>();
		if (OwnerETGM_H) { OwnerETGM_H->UpdateSessionSetting(); }
	}
}

bool APlayerController_House::ServerApplyHouseSetting_Validate() { return true; }
void APlayerController_House::ServerApplyHouseSetting_Implementation() { ApplyHouseSetting(); }


void APlayerController_House::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateUpdateDelegate.Broadcast(PlayerState);
}