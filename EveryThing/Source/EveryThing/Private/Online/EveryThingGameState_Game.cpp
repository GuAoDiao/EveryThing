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

	// TODO: close debug
	// DefaultReadyTime = 15.f;
	CurrentPlayerNum = 0;
	bIsETGameStarted = false;

	DefaultReadyTime = 1.f;
	DefaultGameTime = 600.f;
	DefaultBackToHouseTime = 10.f;
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

	RemaningReadyTime = DefaultReadyTime;
	OnRemaningReadyTimeUpdate();

	UWorld* World = GetWorld();
	if (World) { World->GetTimerManager().SetTimer(ReadyCountDownTimer, this, &AEveryThingGameState_Game::ReadyCountDown, 1.f, true); }
}

void AEveryThingGameState_Game::ReadyCountDown()
{
	if (!HasAuthority()) { return; }
	
	if (RemaningReadyTime <= 0.f)
	{
		UWorld* World = GetWorld();
		if (World) { World->GetTimerManager().ClearTimer(ReadyCountDownTimer); }

		ToggleToTargetETGameState(EETGameState::Gameing);
	}
	else
	{
		RemaningReadyTime -= 1.f;
		OnRemaningReadyTimeUpdate();
	}
}


/// Game

void AEveryThingGameState_Game::StartGameTimeCountDown()
{
	if (!HasAuthority()) { return; }

	RemaningGameTime = DefaultGameTime;
	OnRemaningGameTimeUpdate();

	UWorld* World = GetWorld();
	if (World) { World->GetTimerManager().SetTimer(GameTimeCountDownTimer, this, &AEveryThingGameState_Game::GameTimeCountDown, 1.f, true); }
}
void AEveryThingGameState_Game::GameTimeCountDown()
{
	if (!HasAuthority()) { return; }
	
	if (RemaningGameTime <= 0.f)
	{
		UWorld* World = GetWorld();
		if (World) { World->GetTimerManager().ClearTimer(GameTimeCountDownTimer); }

		ToggleToTargetETGameState(EETGameState::GameOver);
	}
	else
	{
		RemaningGameTime -= 1.f;
		OnRemaningGameTimeUpdate();
	}
}

/// Back to house
void AEveryThingGameState_Game::StartBackToHouseCountDown()
{
	if (!HasAuthority()) { return; }

	RemaningBackToHouseTime = DefaultBackToHouseTime;
	OnRemaningBackToHouseTimeUpdate();

	UWorld* World = GetWorld();
	if (World) { World->GetTimerManager().SetTimer(BackToHouseTimeCountDownTimer, this, &AEveryThingGameState_Game::BackToHouseeCountDown, 1.f, true); }
}

void AEveryThingGameState_Game::BackToHouseeCountDown()
{
	if (!HasAuthority()) { return; }

	if (RemaningBackToHouseTime <= 0.f)
	{
		UWorld* World = GetWorld();
		if (World) { World->GetTimerManager().ClearTimer(BackToHouseTimeCountDownTimer); }

		AEveryThingGameMode_Game* OwnerETGM_G = GetWorld() ? GetWorld()->GetAuthGameMode<AEveryThingGameMode_Game>() : nullptr;
		if (OwnerETGM_G) { OwnerETGM_G->HandlerETBackToHouse(); }
	}
	else
	{
		RemaningBackToHouseTime -= 1.f;
		OnRemaningBackToHouseTimeUpdate();
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
	AEveryThingPlayerState_Game* TargetETPS = ChatPlayerState.IsValidIndex(PlayerID) ? Cast<AEveryThingPlayerState_Game>(ChatPlayerState[PlayerID]) : nullptr;
	if (TargetETPS)
	{
		return TargetETPS->GetPlayerChatName();
	}

	return PlayerChatName_NONE;
}





void AEveryThingGameState_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME(AEveryThingGameState_Game, CurrentETGameState);
	DOREPLIFETIME(AEveryThingGameState_Game, bIsETGameStarted);
	
	DOREPLIFETIME(AEveryThingGameState_Game, RemaningReadyTime);
	DOREPLIFETIME(AEveryThingGameState_Game, RemaningGameTime);
	DOREPLIFETIME(AEveryThingGameState_Game, RemaningBackToHouseTime);

	


	DOREPLIFETIME(AEveryThingGameState_Game, GameType);
	DOREPLIFETIME(AEveryThingGameState_Game, MapName);
	DOREPLIFETIME(AEveryThingGameState_Game, HouseName);
	DOREPLIFETIME(AEveryThingGameState_Game, bIsLANMatach);
	DOREPLIFETIME(AEveryThingGameState_Game, MaxPlayerNum);
	DOREPLIFETIME(AEveryThingGameState_Game, CurrentPlayerNum);

	DOREPLIFETIME(AEveryThingGameState_Game, ChatPlayerState);	
}