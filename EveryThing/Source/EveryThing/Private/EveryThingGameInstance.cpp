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
void UEveryThingGameInstance::OpenGameLevel(const FString& MapType, const FString& MapName)
{
	UGameplayStatics::OpenLevel(this, *MapName, true, TEXT("listen"));
}

void UEveryThingGameInstance::OpenMenuLevel()
{
	UGameplayStatics::OpenLevel(this, MenuLevelName, true);

	APlayerController* OwnerPC = GetFirstLocalPlayerController();
	if (OwnerPC)
	{
		FInputModeUIOnly InputMode;
		OwnerPC->SetInputMode(InputMode);
	}
}


AEveryThingGameSession* UEveryThingGameInstance::GetGameSession()
{
	AGameModeBase* OwnerGameMode = GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr;
	return OwnerGameMode ? Cast<AEveryThingGameSession>(OwnerGameMode->GameSession) : nullptr;
}

void UEveryThingGameInstance::HostGame(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum)
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		// create unique session name
		FName SessionName = FName(*FString(UserId->ToString() + TEXT("_Game")));
		if (UserId.IsValid() && OwnerETGS)
		{
			OwnerETGS->HostSession(*UserId, SessionName, HouseName, GameType, MapName, bIsLAN, bIsPresence, MaxPlayersNum);
		}
	}
}

void UEveryThingGameInstance::FindHoustList(bool bIsLAN, bool bIsPresence)
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		OwnerETGS->FindSessions(*UserId, bIsLAN, bIsPresence);
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
			OwnerETGS->JoinSession(*UserId, SessionResult);
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
			OwnerETGS->JoinSession(*UserId, SearchResultIndex);
		}
	}
}

void UEveryThingGameInstance::ExitGame()
{
	APlayerController* OwnerPC = GetFirstLocalPlayerController();
	if (OwnerPC) { OwnerPC->ConsoleCommand("quit"); }
}