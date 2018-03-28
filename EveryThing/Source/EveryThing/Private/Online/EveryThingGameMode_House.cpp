// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameMode_House.h"

#include "Engine/World.h"

#include "EveryThingGameSession.h"
#include "OnlineSubsystemSessionSettings.h"

#include "Online/EveryThingPlayerState_House.h"
#include "Online/EveryThingGameState_House.h"
#include "Online/PlayerController_House.h"
#include "UI/EveryThingHUD_House.h"

AEveryThingGameMode_House::AEveryThingGameMode_House()
{
	GameSessionClass = AEveryThingGameSession::StaticClass();
	PlayerStateClass = AEveryThingPlayerState_House::StaticClass();
	GameStateClass = AEveryThingGameState_House::StaticClass();
	PlayerControllerClass = APlayerController_House::StaticClass();
	HUDClass = AEveryThingHUD_House::StaticClass();
}

void AEveryThingGameMode_House::OpenGameFromHouseOwner()
{
	AEveryThingGameSession* OwnerETGSession = Cast<AEveryThingGameSession>(GameSession);
	if (OwnerETGSession) { OwnerETGSession->StartSession(); }
}


void AEveryThingGameMode_House::BeginPlay()
{
	Super::BeginPlay();

	AEveryThingGameState_House* OwnerETGS_H = GetGameState<AEveryThingGameState_House>();
	if (OwnerETGS_H)
	{
		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
		if (Sessions.IsValid())
		{
			FOnlineSessionSettings* OwnerOnlineSessionSetting = Sessions->GetSessionSettings(NAME_GameSession);
			if (OwnerOnlineSessionSetting)
			{
				OwnerOnlineSessionSetting->Get<FString>(SETTING_GAMEMODE, OwnerETGS_H->GameType);
				OwnerOnlineSessionSetting->Get<FString>(SETTING_MAPNAME, OwnerETGS_H->MapName);
				OwnerOnlineSessionSetting->Get<FString>(FName("HouseName"), OwnerETGS_H->HouseName);
				OwnerETGS_H->bIsLANMatch = OwnerOnlineSessionSetting->bIsLANMatch;
				OwnerETGS_H->MaxPlayerNum = OwnerOnlineSessionSetting->NumPublicConnections;
			}
		}
	}
}

void AEveryThingGameMode_House::UpdateSessionSetting()
{
	AEveryThingGameState_House* OwnerETGS_H = GetGameState<AEveryThingGameState_House>();
	if (OwnerETGS_H)
	{
		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
		if (Sessions.IsValid())
		{
			FOnlineSessionSettings* OwnerOnlineSessionSetting = Sessions->GetSessionSettings(NAME_GameSession);
			if (OwnerOnlineSessionSetting)
			{
				OwnerOnlineSessionSetting->Set(SETTING_GAMEMODE, OwnerETGS_H->GameType);
				OwnerOnlineSessionSetting->Set(SETTING_MAPNAME, OwnerETGS_H->MapName);
				OwnerOnlineSessionSetting->Set(FName("HouseName"), OwnerETGS_H->HouseName);
				OwnerOnlineSessionSetting->bIsLANMatch = OwnerETGS_H->bIsLANMatch;
				OwnerOnlineSessionSetting->NumPublicConnections = OwnerETGS_H->MaxPlayerNum;
			}
		}
	}
}