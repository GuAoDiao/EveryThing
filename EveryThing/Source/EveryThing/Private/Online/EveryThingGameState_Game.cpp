// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameState_Game.h"

#include "UnrealNetwork.h"

#include "Online/EveryThingPlayerState_Game.h"

const FString AEveryThingGameState_Game::PlayerChatName_NONE = FString("NONE");

void AEveryThingGameState_Game::HandleBeginPlay()
{
	Super::HandleBeginPlay();
}


void AEveryThingGameState_Game::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (PlayerState && Cast<IChatWindowPlayerStateInterface>(PlayerState))
	{
		AEveryThingPlayerState_Game* TargetETPS = Cast<AEveryThingPlayerState_Game>(PlayerState);
		if (TargetETPS)
		{
			int32 Index = AllChatPlayerState.Add(PlayerState);
			TargetETPS->SetPlayerChatID(Index);
		}
	}
}

void AEveryThingGameState_Game::RemovePlayerState(APlayerState* PlayerState)
{
	if (PlayerState)
	{
		for (int32 i=0;i<AllChatPlayerState.Num(); ++i)
		{
			if (AllChatPlayerState[i] == PlayerState)
			{
				AllChatPlayerState[i] = nullptr;
			}
		}
	}

	Super::RemovePlayerState(PlayerState);
}

const FString& AEveryThingGameState_Game::GetPlayerChatName(int32 PlayerID) const
{
	if (AllChatPlayerState.IsValidIndex(PlayerID) && AllChatPlayerState[PlayerID])
	{
		AEveryThingPlayerState_Game* TargetETPS = Cast<AEveryThingPlayerState_Game>(AllChatPlayerState[PlayerID]);

		return TargetETPS->GetPlayerChatName();
	}

	return PlayerChatName_NONE;
}



void AEveryThingGameState_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingGameState_Game, AllChatPlayerState);
}