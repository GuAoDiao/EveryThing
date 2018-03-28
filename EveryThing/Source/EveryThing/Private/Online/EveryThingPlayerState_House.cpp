// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingPlayerState_House.h"

#include "UnrealNetwork.h"
#include "Engine/World.h"

#include "Online/EveryThingGameState_House.h"
#include "EveryThingGameInstance.h"

AEveryThingPlayerState_House::AEveryThingPlayerState_House()
{
	bIsReady = false;
	TeamID = -1;
}

void AEveryThingPlayerState_House::BeginPlay()
{
	Super::BeginPlay();

	UEveryThingGameInstance* OwnerETGI = GetWorld() ? Cast<UEveryThingGameInstance>(GetWorld()->GetGameInstance()) : nullptr;
	if (OwnerETGI) { SetPlayerInfo(OwnerETGI->GetPlayerInfo()); }
}

void AEveryThingPlayerState_House::SetPlayerInfo(const FPlayerInfo& InPlayerInfo)
{
	if (GetOwner() && GetOwner()->Role >= ROLE_AutonomousProxy) { ServerSetPlayerInfo(InPlayerInfo); }
}


bool AEveryThingPlayerState_House::ServerSetPlayerInfo_Validate(const FPlayerInfo& InPlayerInfo) { return true; }
void AEveryThingPlayerState_House::ServerSetPlayerInfo_Implementation(const FPlayerInfo& InPlayerInfo) { CurrentPlayerInfo = InPlayerInfo; }

void AEveryThingPlayerState_House::SetIsReady(bool bInIsReady)
{
	bIsReady = bInIsReady;
	OnIsReadyUptate();
}

void AEveryThingPlayerState_House::TooggleReadState()
{
	if (GetOwner() && GetOwner()->Role >= ROLE_AutonomousProxy){ServerTooggleReadState();}
}
bool AEveryThingPlayerState_House::ServerTooggleReadState_Validate() { return true; }
void AEveryThingPlayerState_House::ServerTooggleReadState_Implementation() { SetIsReady(!bIsReady); }

void AEveryThingPlayerState_House::SetTeamID(int32 InTeamID)
{
	if (GetOwner() && GetOwner()->Role >= ROLE_AutonomousProxy) { ServerSetTeamID(InTeamID); }
}
bool AEveryThingPlayerState_House::ServerSetTeamID_Validate(int32 InTeamID) { return true; }
void AEveryThingPlayerState_House::ServerSetTeamID_Implementation(int32 InTeamID) { TeamID = InTeamID; }


bool AEveryThingPlayerState_House::CheckIsHouseOwner()
{
	AEveryThingGameState_House* OwnerETGS_H = GetWorld() ? GetWorld()->GetGameState<AEveryThingGameState_House>() : nullptr;
	return OwnerETGS_H && OwnerETGS_H->CheckPlayerIsHouseOwner(this);
}

void AEveryThingPlayerState_House::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingPlayerState_House, CurrentPlayerInfo);
	DOREPLIFETIME(AEveryThingPlayerState_House, bIsReady);
	DOREPLIFETIME(AEveryThingPlayerState_House, TeamID);
}