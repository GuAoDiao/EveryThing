// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameMode_Game.h"

#include "Engine/World.h"

#include "Online/PlayerController_Game.h"
#include "Online/EveryThingPlayerState_Game.h"
#include "Online/EveryThingGameState_Game.h"
#include "Online/EveryThingGameSession.h"
#include "UI/EveryThingHUD_Game.h"

#include "PlayerFootballPawn.h"

AEveryThingGameMode_Game::AEveryThingGameMode_Game()
{
	NormalDamageScale = 1.f;
	SpecificToDamageScale = 1.5f;
	bStartPlayersAsSpectators = true;
	bPauseable = false;

	DefaultPawnClass = APlayerFootballPawn::StaticClass();
	
	GameSessionClass = AEveryThingGameSession::StaticClass();
	PlayerControllerClass = APlayerController_Game::StaticClass();
	PlayerStateClass = AEveryThingPlayerState_Game::StaticClass();
	GameStateClass = AEveryThingGameState_Game::StaticClass();
	HUDClass = AEveryThingHUD_Game::StaticClass();
}


//////////////////////////////////////////////////////////////////////////
/// Game flow path

void AEveryThingGameMode_Game::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AEveryThingGameMode_Game::InitGameState()
{
	Super::InitGameState();
	
	UE_LOG(LogTemp, Log, TEXT("-_- init game state"));

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	FOnlineSessionSettings* OwnerOnlineSessionSetting = Sessions.IsValid() ? Sessions->GetSessionSettings(NAME_GameSession) : nullptr;
	FNamedOnlineSession* OwnerNamedSession = Sessions->GetNamedSession(NAME_GameSession);
	AEveryThingGameState_Game* OwnerETGS_G = GetGameState<AEveryThingGameState_Game>();
	if (OwnerOnlineSessionSetting && OwnerETGS_G && OwnerNamedSession)
	{		
		FString HouseName, GameType, MapName;
		OwnerOnlineSessionSetting->Get<FString>(SETTING_GAMEMODE, GameType);
		OwnerOnlineSessionSetting->Get<FString>(SETTING_MAPNAME, MapName);
		OwnerOnlineSessionSetting->Get<FString>(FName("HouseName"), HouseName);
		
		int32 MaxPlayersNum = OwnerOnlineSessionSetting->NumPublicConnections;
		int32 CurrentPlayerNum = MaxPlayersNum - OwnerNamedSession->NumOpenPublicConnections;
		
		UE_LOG(LogTemp, Log, TEXT("-_- all palyer: %d"), CurrentPlayerNum);

		OwnerETGS_G->InitializeETGameState(GameType, MapName, HouseName, OwnerOnlineSessionSetting->bIsLANMatch, MaxPlayersNum, CurrentPlayerNum);
	}
}

void AEveryThingGameMode_Game::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	HandlePlayerLogin(NewPlayer);
}


void AEveryThingGameMode_Game::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);
	
	HandlePlayerLogin(NewPC);
}

void AEveryThingGameMode_Game::HandlePlayerLogin(APlayerController* NewPlayer)
{
	AEveryThingGameState_Game* OwnerETGS_G = GetGameState<AEveryThingGameState_Game>();
	if (OwnerETGS_G)
	{
		OwnerETGS_G->OnPlayerPostLogin(NewPlayer);

		if (OwnerETGS_G->IsETGameStarted())
		{
			RestartPlayer(NewPlayer);
		}
		else
		{
			APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(NewPlayer);
			if (OwnerPC_G)
			{
				switch (OwnerETGS_G->GetETGameState())
				{
					case EETGameState::WaitForHousePlayerLoad:
						OwnerPC_G->ClientWaitForHousePlayerLoad();
						break;
					case EETGameState::ReadyToStart:
						OwnerPC_G->ClientReadyToStart();
						break;
				}
			}
		}
	}
}

void AEveryThingGameMode_Game::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(NewPlayer);
	if (OwnerPC_G)
	{
		OwnerPC_G->ClientStartPlayer();
	}
}

void AEveryThingGameMode_Game::Logout(AController* Exiting)
{
	AEveryThingGameState_Game* OwnerETGS_G = GetGameState<AEveryThingGameState_Game>();
	if (OwnerETGS_G)
	{
		OwnerETGS_G->OnPlayerLogout(Exiting);
	}

	Super::Logout(Exiting);
}

void AEveryThingGameMode_Game::HandleETGameReay()
{
	AEveryThingGameState_Game* OwnerETGS_G = GetGameState<AEveryThingGameState_Game>();
	if (OwnerETGS_G)
	{
		OwnerETGS_G->StartReadyCountDown();
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(It->Get());
		if (OwnerPC_G) { OwnerPC_G->ClientReadyToStart(); }
	}
}
void AEveryThingGameMode_Game::HandleETGameStart()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		RestartPlayer(It->Get());
	}
}
bool AEveryThingGameMode_Game::HandleETGameIsOver()
{
	return true;
}
void AEveryThingGameMode_Game::HandleETGameOver()
{

}



//////////////////////////////////////////////////////////////////////////
/// Damage


float AEveryThingGameMode_Game::GetActualDamage(float InDamage, EElementType InDamageType, EElementType PawnElementType, float ElementResistance) const
{
	return InDamage * GetDamageScaleFromElementType(InDamageType, PawnElementType);
}

float AEveryThingGameMode_Game::GetDamageScaleFromElementType(EElementType CauserElementType, EElementType AcceptElementType) const
{
	switch (CauserElementType)
	{
		case EElementType::Fire:
			if (AcceptElementType == EElementType::Water) { return SpecificToDamageScale; }
			break;
		case EElementType::Water:
			if (AcceptElementType == EElementType::Fire) { return SpecificToDamageScale; }
			break;
	}

	return NormalDamageScale;
}

void AEveryThingGameMode_Game::BeginDestroy()
{

	AEveryThingGameSession* OwnerGameSession = Cast<AEveryThingGameSession>(GameSession);
	if (OwnerGameSession) { OwnerGameSession->DestroySession(); }

	Super::BeginDestroy();
}