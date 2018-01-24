// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameInstance.h"

#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"


#include "Online/EveryThingGameMode_Menu.h"
#include "Online/EveryThingGameSession.h"

UEveryThingGameInstance::UEveryThingGameInstance()
{

}


//////////////////////////////////////////////////////////////////////////
/// Level
void UEveryThingGameInstance::OpenGameLevel(const FName& LevelName)
{
	UGameplayStatics::OpenLevel(this, LevelName, true, TEXT("listen"));
}

void UEveryThingGameInstance::OpenMenuLevel()
{
	UGameplayStatics::OpenLevel(this, MenuLevelName, true);
}


AEveryThingGameSession* UEveryThingGameInstance::GetGameSession()
{
	AEveryThingGameMode_Menu* OwnerMenuETGM = GetWorld()->GetAuthGameMode<AEveryThingGameMode_Menu>();
	return Cast<AEveryThingGameSession>(OwnerMenuETGM->GameSession);
}

void UEveryThingGameInstance::HostGame(const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum)
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		if (UserId.IsValid() && OwnerETGS)
		{
			OwnerETGS->HostSession(*UserId, GameSessionName, GameType, MapName, bIsLAN, bIsPresence, MaxPlayersNum);
		}
	}
}

void UEveryThingGameInstance::FindHoustList()
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		OwnerETGS->FindSessions(*UserId, GameSessionName, true, true);
	}
}

void UEveryThingGameInstance::JoinGame(FOnlineSessionSearchResult& SessionResult)
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		if (UserId.IsValid() && SessionResult.Session.OwningUserId != UserId)
		{
			OwnerETGS->JoinSession(*UserId, *SessionResult.Session.OwningUserName, SessionResult);
		}
	}
}

void UEveryThingGameInstance::JoinGame(FName SessionName, int32 SearchResultIndex)
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		if (UserId.IsValid())
		{
			OwnerETGS->JoinSession(*UserId, SessionName, SearchResultIndex);
		}
	}
}

void UEveryThingGameInstance::ExitGame()
{
	APlayerController* OwnerPC = GetFirstLocalPlayerController();
	if (OwnerPC) { OwnerPC->ConsoleCommand("quit"); }
}