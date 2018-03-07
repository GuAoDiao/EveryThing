// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameState.h"

#include "UnrealNetwork.h"

#include "Online/EveryThingPlayerState.h"

const FString AEveryThingGameState::PlayerChatName_NONE = FString("NONE");

void AEveryThingGameState::HandleBeginPlay()
{
	Super::HandleBeginPlay();
}


void AEveryThingGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (PlayerState && Cast<IChatWindowPlayerStateInterface>(PlayerState))
	{
		AEveryThingPlayerState* TargetETPS = Cast<AEveryThingPlayerState>(PlayerState);
		if (TargetETPS)
		{
			int32 Index = AllChatPlayerState.Add(PlayerState);
			TargetETPS->SetPlayerChatID(Index);
		}
	}
}

void AEveryThingGameState::RemovePlayerState(APlayerState* PlayerState)
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

const FString& AEveryThingGameState::GetPlayerChatName(int32 PlayerID) const
{
	if (AllChatPlayerState.IsValidIndex(PlayerID) && AllChatPlayerState[PlayerID])
	{
		AEveryThingPlayerState* TargetETPS = Cast<AEveryThingPlayerState>(AllChatPlayerState[PlayerID]);

		return TargetETPS->GetPlayerChatName();
	}

	return PlayerChatName_NONE;
}



void AEveryThingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingGameState, AllChatPlayerState);
}