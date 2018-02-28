// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameInstance.h"

#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/OnlineSession.h"

#include "Online/EveryThingGameMode_Menu.h"
#include "Online/EveryThingGameSession.h"

#include "EveryThingSaveGame.h"

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
}


void UEveryThingGameInstance::ExitGameApplication()
{
	APlayerController* OwnerPC = GetFirstLocalPlayerController();
	if (OwnerPC) { OwnerPC->ConsoleCommand("quit"); }
}

//////////////////////////////////////////////////////////////////////////
/// Match
void UEveryThingGameInstance::HostGame(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum)
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		if (UserId.IsValid()) { OwnerETGS->HostSession(*UserId, NAME_GameSession, HouseName, GameType, MapName, bIsLAN, bIsPresence, MaxPlayersNum); }
	}
}

void UEveryThingGameInstance::FindHoustList(bool bIsLAN, bool bIsPresence)
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		if (UserId.IsValid()) { OwnerETGS->FindSessions(*UserId, bIsLAN, bIsPresence); }
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

void UEveryThingGameInstance::JoinGame(int32 SearchResultIndex)
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		if (UserId.IsValid()) { OwnerETGS->JoinSession(*UserId, SearchResultIndex); }
	}
}

void UEveryThingGameInstance::ExitGame()
{
	bool bSuccess = false;
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	if (OwnerETGS)
	{
		bSuccess = OwnerETGS->DestroySession();
	}
	else
	{
		UOnlineSession* OwnerOnlineSession = GetOnlineSession();
		if (OwnerOnlineSession)
		{
			OwnerOnlineSession->EndOnlineSession(NAME_GameSession);
			bSuccess = true;
		}
	}

	if (bSuccess) { OpenMenuLevel(); }
}

AEveryThingGameSession* UEveryThingGameInstance::GetGameSession()
{
	AGameModeBase* OwnerGameMode = GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr;
	return OwnerGameMode ? Cast<AEveryThingGameSession>(OwnerGameMode->GameSession) : nullptr;
}


bool UEveryThingGameInstance::LoadPlayerInfoFromSlotName(const FString& SlotName)
{
	UEveryThingSaveGame* CurrentSaveGame = Cast<UEveryThingSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (CurrentSaveGame)
	{
		SaveDataSlotName = SlotName;
		SetPlayerInfo(CurrentSaveGame->PlayerInfo);
		return true;
	}

	return false;
}

bool UEveryThingGameInstance::SavePlayerInfoToCurrentSlotName()
{
	return SavePlayerInfoWithSlotName(SaveDataSlotName);
}

bool UEveryThingGameInstance::SavePlayerInfoWithSlotName(const FString& SlotName)
{
	UEveryThingSaveGame* CurrentSaveGame = Cast<UEveryThingSaveGame>(UGameplayStatics::CreateSaveGameObject(UEveryThingSaveGame::StaticClass()));;
	if (CurrentSaveGame)
	{
		if (UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0))
		{
			SaveDataSlotName = SlotName;
			return true;
		}
	}

	return false;
}