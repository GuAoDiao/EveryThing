// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameState_Game.h"

#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "TimerManager.h"

#include "Online/EveryThingPlayerState_Game.h"
#include "Online/EveryThingGameMode_Game.h"

const FString AEveryThingGameState_Game::PlayerChatName_NONE = FString("NONE");

AEveryThingGameState_Game::AEveryThingGameState_Game()
{
	CurrentETGameState = EETGameState::WaitForHousePlayerLoad;
	DefaultReadyTime = 15.f;
	CurrentPlayerNum = 0;
	bIsETGameStarted = false;
}

//////////////////////////////////////////////////////////////////////////
/// Game flow path
void AEveryThingGameState_Game::InitializeETGameState(const FString& InGameType, const FString& InMapName, const FString& InHouseName, bool bInIsLANMatach, int32 InMaxPlayerNum, int32 InCurrentPlayerNum)
{
	if (!HasAuthority()) { return; }

	GameType = InGameType;
	MapName = InMapName;
	HouseName = InHouseName;
	bIsLANMatach = bInIsLANMatach;
	MaxPlayerNum = InMaxPlayerNum;
	HousePlayerNum = InCurrentPlayerNum;
}

void AEveryThingGameState_Game::StartReadyCountDown()
{
	if (!HasAuthority()) { return; }

	ReadyRemaningTime = DefaultReadyTime;
	OnReadyRemaningTimeUpdate();

	UWorld* World = GetWorld();
	if (World) { World->GetTimerManager().SetTimer(ReadyCountDownTimer, this, &AEveryThingGameState_Game::ReadyCountDown, 1.f, false); }
}

void AEveryThingGameState_Game::ReadyCountDown()
{
	if (!HasAuthority()) { return; }

	UE_LOG(LogTemp, Log, TEXT("-_- ReadRemaningTime : %f"), ReadyRemaningTime);

	if (ReadyRemaningTime <= 0.f)
	{
		ToggleToTargetETGameState(EETGameState::Gameing);
	}
	else
	{
		ReadyRemaningTime -= 1.f;
		OnReadyRemaningTimeUpdate();

		UWorld* World = GetWorld();
		if (World) { World->GetTimerManager().SetTimer(ReadyCountDownTimer, this, &AEveryThingGameState_Game::ReadyCountDown, 1.f, false); }
	}
}

//////////////////////////////////////////////////////////////////////////
/// Player State
void AEveryThingGameState_Game::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (!HasAuthority()) { return; }

	if (PlayerState && Cast<IChatWindowPlayerStateInterface>(PlayerState))
	{
		AEveryThingPlayerState_Game* TargetETPS = Cast<AEveryThingPlayerState_Game>(PlayerState);
		if (TargetETPS)
		{			
			int32 Index = PlayerArray.Find(PlayerState);
			TargetETPS->SetPlayerChatID(Index);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/// Game State

void AEveryThingGameState_Game::OnPlayerPostLogin(APlayerController* NewPlayer)
{
	if (!HasAuthority()) { return; }

	++CurrentPlayerNum;

	UE_LOG(LogTemp, Log, TEXT("-_- On Player Add, Current Player: %d / %d"), CurrentPlayerNum, HousePlayerNum);

	if (IsTargetETGameState(EETGameState::WaitForHousePlayerLoad) && CurrentPlayerNum >= HousePlayerNum)
	{
		ToggleToTargetETGameState(EETGameState::ReadyToStart);
	}
}

void AEveryThingGameState_Game::OnPlayerLogout(AController* Exiting)
{
	if (!HasAuthority()) { return; }

	--CurrentPlayerNum;

	if (IsTargetETGameState(EETGameState::WaitForHousePlayerLoad)) { --HousePlayerNum; }
}

void AEveryThingGameState_Game::ToggleToTargetETGameState(EETGameState TargetGameStae)
{

	CurrentETGameState = TargetGameStae;

	if (!HasAuthority()) { return; }
	switch (TargetGameStae)
	{
		case EETGameState::WaitForHousePlayerLoad:
		case EETGameState::ReadyToStart:
		{
			UE_LOG(LogTemp, Log, TEXT("-_- toggle game state to GameReady"));
			AEveryThingGameMode_Game* OwnerETGM_G = GetWorld() ? GetWorld()->GetAuthGameMode<AEveryThingGameMode_Game>() : nullptr;
			if (OwnerETGM_G) { OwnerETGM_G->HandleETGameReay(); }
			break;
		}
		case EETGameState::Gameing:
		{
			bIsETGameStarted = true;

			UE_LOG(LogTemp, Log, TEXT("-_- toggle game state to Gameing"));
			AEveryThingGameMode_Game* OwnerETGM_G = GetWorld() ? GetWorld()->GetAuthGameMode<AEveryThingGameMode_Game>() : nullptr;
			if (OwnerETGM_G) { OwnerETGM_G->HandleETGameStart(); }
			break;
		}
		case EETGameState::GameOver:
		{
			UE_LOG(LogTemp, Log, TEXT("-_- toggle game state to GameOver"));
			AEveryThingGameMode_Game* OwnerETGM_G = GetWorld() ? GetWorld()->GetAuthGameMode<AEveryThingGameMode_Game>() : nullptr;
			if (OwnerETGM_G) { OwnerETGM_G->HandleETGameOver(); }
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/// Chat
const FString& AEveryThingGameState_Game::GetPlayerChatName(int32 PlayerID) const
{
	
	AEveryThingPlayerState_Game* TargetETPS = PlayerArray.IsValidIndex(PlayerID) ? Cast<AEveryThingPlayerState_Game>(PlayerArray[PlayerID]) : nullptr;
	if (TargetETPS)
	{
		return TargetETPS->GetPlayerChatName();
	}

	return PlayerChatName_NONE;
}





void AEveryThingGameState_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingGameState_Game, ReadyRemaningTime);
	
	DOREPLIFETIME(AEveryThingGameState_Game, CurrentETGameState);
	DOREPLIFETIME(AEveryThingGameState_Game, bIsETGameStarted);
	
	DOREPLIFETIME(AEveryThingGameState_Game, GameType);
	DOREPLIFETIME(AEveryThingGameState_Game, MapName);
	DOREPLIFETIME(AEveryThingGameState_Game, HouseName);
	DOREPLIFETIME(AEveryThingGameState_Game, bIsLANMatach);
	DOREPLIFETIME(AEveryThingGameState_Game, MaxPlayerNum);
	DOREPLIFETIME(AEveryThingGameState_Game, CurrentPlayerNum);
}