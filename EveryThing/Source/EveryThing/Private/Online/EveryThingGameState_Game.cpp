// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameState_Game.h"

#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "TimerManager.h"

#include "Characters/GamePawn.h"
#include "Online/EveryThingPlayerState_Game.h"
#include "Online/EveryThingGameMode_Game.h"
#include "Online/PlayerController_Game.h"

const FString AEveryThingGameState_Game::PlayerChatName_NONE = FString("NONE");

AEveryThingGameState_Game::AEveryThingGameState_Game()
{
	CurrentETGameState = EETGameState::WaitForHousePlayerLoad;

	// TODO: close debug
	// DefaultReadyTime = 15.f;
	CurrentPlayerNum = 0;
	ActualTeamNums = 1;
	bIsETGameStarted = false;

	DefaultReadyTime = 1.f;
	DefaultGameTime = 600.f;
	DefaultBackToHouseTime = 10.f;

	// score
	CureScoreScale = 0.5f;
	DamageScoreScale = 0.7f;
	CriticalDamageScore = 20.f;
	KillScore = 100.f;
}

//////////////////////////////////////////////////////////////////////////
/// Game flow path
void AEveryThingGameState_Game::InitializeETGameState(const FString& InGameType, const FString& InMapName, const FString& InHouseName, bool bInIsLANMatach, int32 InMaxPlayerNum, int32 InCurrentPlayerNum)
{
	GameType = InGameType;
	MapName = InMapName;
	HouseName = InHouseName;
	bIsLANMatach = bInIsLANMatach;
	MaxPlayerNum = InMaxPlayerNum;
	HousePlayerNum = InCurrentPlayerNum;
}

void AEveryThingGameState_Game::StartReadyCountDown()
{
	RemaningReadyTime = DefaultReadyTime;
	OnRemaningReadyTimeUpdate();

	GetWorldTimerManager().SetTimer(CountDownTimer, this, &AEveryThingGameState_Game::ReadyCountDown, 1.f, true);
}

void AEveryThingGameState_Game::ReadyCountDown()
{
	if (RemaningReadyTime <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(CountDownTimer);

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
	RemaningGameTime = DefaultGameTime;
	OnRemaningGameTimeUpdate();

	// TODO: Need to optimize
	GetWorldTimerManager().SetTimer(CountDownTimer, this, &AEveryThingGameState_Game::GameTimeCountDown, 1.f, true);
}
void AEveryThingGameState_Game::GameTimeCountDown()
{
	if (RemaningGameTime <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(CountDownTimer);

		ToggleToTargetETGameState(EETGameState::GameOver);
	}
	else
	{
		RemaningGameTime -= 1.f;
		OnRemaningGameTimeUpdate();
	}
}

/// Back to house
void AEveryThingGameState_Game::StartBackToHouseTimeCountDown()
{
	RemaningBackToHouseTime = DefaultBackToHouseTime;
	OnRemaningBackToHouseTimeUpdate();

	GetWorldTimerManager().SetTimer(CountDownTimer, this, &AEveryThingGameState_Game::BackToHouseeTimeCountDown, 1.f, true);
}

void AEveryThingGameState_Game::BackToHouseeTimeCountDown()
{
	if (RemaningBackToHouseTime <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(CountDownTimer);

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
	++CurrentPlayerNum;

	UE_LOG(LogTemp, Log, TEXT("-_- On Player Add, Current Player: %d / %d"), CurrentPlayerNum, HousePlayerNum);

	if (IsTargetETGameState(EETGameState::WaitForHousePlayerLoad) && CurrentPlayerNum >= HousePlayerNum)
	{
		ToggleToTargetETGameState(EETGameState::ReadyToStart);
	}
}

void AEveryThingGameState_Game::OnPlayerLogout(AController* Exiting)
{
	--CurrentPlayerNum;

	if (IsTargetETGameState(EETGameState::WaitForHousePlayerLoad)) { --HousePlayerNum; }
}

void AEveryThingGameState_Game::ToggleToTargetETGameState(EETGameState TargetGameStae)
{
	CurrentETGameState = TargetGameStae;

	AEveryThingGameMode_Game* OwnerETGM_G = GetWorld() ? GetWorld()->GetAuthGameMode<AEveryThingGameMode_Game>() : nullptr;
	if (OwnerETGM_G)
	{
		switch (TargetGameStae)
		{
			case EETGameState::WaitForHousePlayerLoad:
				break;
			case EETGameState::ReadyToStart: {OwnerETGM_G->HandleETGameReay(); break; }
			case EETGameState::Gameing:
			{
				bIsETGameStarted = true;
				OwnerETGM_G->HandleETGameStart(); break;
			}
			case EETGameState::GameOver: {OwnerETGM_G->HandleETGameOver(); break; }
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


//////////////////////////////////////////////////////////////////////////
/// Game Pawn Damage And Death
void AEveryThingGameState_Game::OnGamePawnAcceptCure(AGamePawn* AccpetPawn, AActor* Causer, float Treatment)
{
	APlayerController_Game* AcceptPC_G = AccpetPawn ? Cast<APlayerController_Game>(AccpetPawn->GetController()) : nullptr;
	if (AcceptPC_G) { AcceptPC_G->ClientOnAcceptCure(Causer, Treatment); }
	
	AGamePawn* CauserGamePawn = Cast<AGamePawn>(Causer);
	APlayerController_Game* CasuerPC_G = CauserGamePawn ? Cast<APlayerController_Game>(CauserGamePawn->GetController()) : nullptr;
	if (CasuerPC_G)
	{
		AEveryThingPlayerState_Game* CauserETPS_G = Cast<AEveryThingPlayerState_Game>(CasuerPC_G->PlayerState);
		if (CauserETPS_G) { CauserETPS_G->AddGameScore(Treatment * CureScoreScale); }

		CasuerPC_G->ClientOnTakeCure(AccpetPawn, Treatment);
	}
}
void AEveryThingGameState_Game::OnGamePawnAcceptDamage(AGamePawn* AccpetPawn, AActor* Causer, float Damage)
{
	APlayerController_Game* AcceptPC_G = AccpetPawn ? Cast<APlayerController_Game>(AccpetPawn->GetController()) : nullptr;
	if (AcceptPC_G) { AcceptPC_G->ClientOnAcceptDamage(Causer, Damage); }

	AGamePawn* CauserGamePawn = Cast<AGamePawn>(Causer);
	APlayerController_Game* CasuerPC_G = CauserGamePawn ? Cast<APlayerController_Game>(CauserGamePawn->GetController()) : nullptr;
	if (CasuerPC_G)
	{
		AEveryThingPlayerState_Game* CauserETPS_G = Cast<AEveryThingPlayerState_Game>(CasuerPC_G->PlayerState);
		if (CauserETPS_G) { CauserETPS_G->AddGameScore(Damage * DamageScoreScale); }

		CasuerPC_G->ClientOnTakeDamage(AccpetPawn, Damage);
	}
}

void AEveryThingGameState_Game::OnGamePawnAcceptCriticalDamage(AGamePawn* AccpetPawn, AActor* Causer)
{
	APlayerController_Game* AcceptPC_G = AccpetPawn ? Cast<APlayerController_Game>(AccpetPawn->GetController()) : nullptr;
	if (AcceptPC_G) { AcceptPC_G->ClientOnAcceptCriticalDamage(Causer); }

	AGamePawn* CauserGamePawn = Cast<AGamePawn>(Causer);
	APlayerController_Game* CasuerPC_G = CauserGamePawn ? Cast<APlayerController_Game>(CauserGamePawn->GetController()) : nullptr;
	if (CasuerPC_G)
	{
		AEveryThingPlayerState_Game* CauserETPS_G = Cast<AEveryThingPlayerState_Game>(CasuerPC_G->PlayerState);
		if (CauserETPS_G) { CauserETPS_G->AddGameScore(CriticalDamageScore); }

		CasuerPC_G->ClientOnTakeCriticalDamage(AccpetPawn);
	}
}

void AEveryThingGameState_Game::OnGamePawnBeKilled(AGamePawn* KilledActor, AActor* KillerActor)
{
	APlayerController_Game* KilledPC_G = KilledActor ? Cast<APlayerController_Game>(KilledActor->GetController()) : nullptr;
	if (KilledPC_G)
	{
		AEveryThingPlayerState_Game* KilledETPS_G = Cast<AEveryThingPlayerState_Game>(KilledPC_G->PlayerState);
		if (KilledETPS_G) { KilledETPS_G->IncDeathNum(); }

		KilledPC_G->ClientOnBeKilled(KillerActor);

		AEveryThingGameMode_Game* OwnerETGM_G = GetWorld() ? GetWorld()->GetAuthGameMode<AEveryThingGameMode_Game>() : nullptr;
		if (OwnerETGM_G)
		{
			KilledPC_G->Possess(nullptr);
			KilledActor->Destroy();
			OwnerETGM_G->RestartPlayer(KilledPC_G);
		}
	}

	AGamePawn* KillerGamePawn = Cast<AGamePawn>(KillerActor);
	APlayerController_Game* KillerPC_G = KillerGamePawn ? Cast<APlayerController_Game>(KillerGamePawn->GetController()) : nullptr;
	if (KillerPC_G)
	{
		AEveryThingPlayerState_Game* KillerETPS_G = Cast<AEveryThingPlayerState_Game>(KillerPC_G->PlayerState);
		if (KillerETPS_G)
		{
			KillerETPS_G->AddGameScore(KillScore);
			KillerETPS_G->IncKillNum();
		}

		KillerPC_G->ClientOnKillOther(KilledActor);
	}
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