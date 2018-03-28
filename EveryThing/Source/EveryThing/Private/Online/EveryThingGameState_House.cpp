// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameState_House.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"

#include "Online/EveryThingPlayerState_House.h"

void AEveryThingGameState_House::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && GetWorld())
	{
		APlayerController* OwnerPC = GetWorld()->GetFirstPlayerController();
		HouseOwner = OwnerPC ? Cast<AEveryThingPlayerState_House>(OwnerPC->PlayerState) : nullptr;
		if (HouseOwner) { HouseOwner->SetIsReady(true); }
	}
}

void AEveryThingGameState_House::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	OnAddPlayerDelegate.Broadcast(PlayerState);
}

bool AEveryThingGameState_House::CheckIsAllPlayerAreReady()
{	
	for (APlayerState* Player : PlayerArray)
	{
		AEveryThingPlayerState_House* CurrentETPS_H = Cast<AEveryThingPlayerState_House>(Player);
		if (!CurrentETPS_H) { return false; }
		if (!CurrentETPS_H->CheckIsHouseOwner() && !CurrentETPS_H->GetIsReady()) { return false; }
	}


	return true;
}






void AEveryThingGameState_House::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingGameState_House, GameType);
	DOREPLIFETIME(AEveryThingGameState_House, MapName);
	DOREPLIFETIME(AEveryThingGameState_House, HouseName);
	DOREPLIFETIME(AEveryThingGameState_House, bIsLANMatch);
	DOREPLIFETIME(AEveryThingGameState_House, MaxPlayerNum);
	DOREPLIFETIME(AEveryThingGameState_House, CurrentHouseName);
}