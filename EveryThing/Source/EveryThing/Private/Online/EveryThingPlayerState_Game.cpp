// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingPlayerState_Game.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"

#include "EveryThingGameInstance.h"
#include "Online/EveryThingPlayerState_House.h"
#include "Online/EveryThingGameState_Game.h"

#include "Characters/GamePawn.h"
#include "Characters/PlayerPawns/PlayerChairPawn.h"
#include "Characters/PlayerPawns/PlayerFootballPawn.h"

void AEveryThingPlayerState_Game::BeginPlay()
{
	Super::BeginPlay();

	GameScore = 100.f;
	
	// if not from SeamlessTravel, add in the middle of th game. set player info from GameInstance.
	UEveryThingGameInstance* OwnerETGI = Cast<UEveryThingGameInstance>(GetGameInstance());
	APlayerController* OwnerPC = Cast<APlayerController>(GetOwner());
	if (OwnerETGI && OwnerPC && OwnerPC->IsLocalController() && !bFromPreviousLevel)
	{
		ServerSetPlayerInfo( OwnerETGI->GetPlayerInfo());
	}



	if (HasAuthority())
	{
		AEveryThingGameState_Game* OwnerETGS_G = GetWorld() ? GetWorld()->GetGameState<AEveryThingGameState_Game>() : nullptr;
		if (OwnerETGS_G) { ChatID = OwnerETGS_G->GetNextChatID(this); }
	}
}

void AEveryThingPlayerState_Game::SeamlessTravelTo(class APlayerState* NewPlayerState)
{
	Super::SeamlessTravelTo(NewPlayerState);

	// move old info to new
	AEveryThingPlayerState_House* OldETPS_G = Cast<AEveryThingPlayerState_House>(NewPlayerState);
	if (OldETPS_G)
	{
		OldETPS_G->SetPlayerInfo(CurrentPlayerInfo);
		OldETPS_G->SetTeamID(TeamID);
	}
}

void AEveryThingPlayerState_Game::SetPlayerInfo(const FPlayerInfo& InPlayerInfo)
{
	CurrentPlayerInfo = InPlayerInfo;
	OnPlayerInfoUpdate();
}

void AEveryThingPlayerState_Game::ChangeTeamID(int32 InTeamID)
{
	TeamID = InTeamID;
	OnTeamIDUpdate();
}


void AEveryThingPlayerState_Game::AddGameScore(int32 InOffset)
{
	GameScore += InOffset;
	OnGameScoreOffsetDelegate.Broadcast(InOffset);
	OnGameScoreUpdateDelegate.Broadcast(GameScore);
}


bool AEveryThingPlayerState_Game::ServerSetPlayerInfo_Validate(const FPlayerInfo& InPlayerInfo) { return true; }
void AEveryThingPlayerState_Game::ServerSetPlayerInfo_Implementation(const FPlayerInfo& InPlayerInfo) { SetPlayerInfo(InPlayerInfo); }

void AEveryThingPlayerState_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingPlayerState_Game, CurrentPlayerInfo);
	DOREPLIFETIME(AEveryThingPlayerState_Game, TeamID);

	DOREPLIFETIME(AEveryThingPlayerState_Game, ChatID);
}