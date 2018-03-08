// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameInstance.h"

#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/OnlineSession.h"

#include "MoviePlayer.h"

#include "Online/EveryThingGameMode_Menu.h"
#include "Online/EveryThingGameSession.h"

#include "UI/LoadingMap.h"

#include "EveryThingAssetManager.h"
#include "EveryThingSaveArchive.h"
#include "EveryThingSaveArchivesList.h"

UEveryThingGameInstance::UEveryThingGameInstance()
{
}

void UEveryThingGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UEveryThingGameInstance::BeginLoadingMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UEveryThingGameInstance::EndLoadingMap);
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
/// Loading Map
void UEveryThingGameInstance::BeginLoadingMap(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		LoadingMapMoviePlayer = GetMoviePlayer();
		if (LoadingMapMoviePlayer)
		{
			if (!LoadingMap)
			{
				TSubclassOf<UUserWidget> LoadingMapClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("LoadingMap");
				LoadingMap = LoadingMapClass ? CreateWidget<ULoadingMap>(this, LoadingMapClass) : nullptr;
			}
			if(LoadingMap)
			{
				LoadingMap->InitializeLoadingMap(this, MapName);

				FLoadingScreenAttributes LoadingScreenAttributes;
				LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = false;
				LoadingScreenAttributes.bMoviesAreSkippable = true;
				LoadingScreenAttributes.bWaitForManualStop = true;
				LoadingScreenAttributes.PlaybackType = EMoviePlaybackType::MT_Looped;
				LoadingScreenAttributes.WidgetLoadingScreen = LoadingMap->TakeWidget();

				// May only support h264 encoded 720P  mp4
				LoadingScreenAttributes.MoviePaths.Add(TEXT("LoadingMap"));

				LoadingMapMoviePlayer->SetupLoadingScreen(LoadingScreenAttributes);
				
			}
		}
	}
}

bool UEveryThingGameInstance::IsLoadingMapFinished() const
{
	return LoadingMapMoviePlayer ? LoadingMapMoviePlayer->IsLoadingFinished() : true;
}

void UEveryThingGameInstance::EndLoadingMap(UWorld* LoadedWorld)
{
	if (!IsRunningDedicatedServer())
	{
		LoadingMapMoviePlayer = nullptr;
	}
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


bool UEveryThingGameInstance::CreateArchive(const FString& ArchiveName, const FString& PlayerName)
{
	UEveryThingSaveArchive* CurrentSaveGame = Cast<UEveryThingSaveArchive>(UGameplayStatics::CreateSaveGameObject(UEveryThingSaveArchive::StaticClass()));;
	if (CurrentSaveGame)
	{
		CurrentSaveGame->PlayerInfo.PlayerName = PlayerName;
		CurrentSaveGame->PlayerInfo.Gold = 1000;
		CurrentSaveGame->PlayerInfo.Experience = 0;
		CurrentSaveGame->PlayerInfo.AllHaveRolesName.Add("Football");

		if (UGameplayStatics::SaveGameToSlot(CurrentSaveGame, ArchiveName, 0))
		{
			CurrentSaveArchiveName = ArchiveName;
			PlayerInfo = CurrentSaveGame->PlayerInfo;
			CurrentArchivesList.Add(ArchiveName);
			UpdateArchivesList();

			return true;
		}
	}

	return false;
}


bool UEveryThingGameInstance::LoadArchiveFromName(const FString& ArchiveName)
{
	UEveryThingSaveArchive* OwnerETSA = Cast<UEveryThingSaveArchive>(UGameplayStatics::LoadGameFromSlot(ArchiveName, 0));
	if (OwnerETSA)
	{
		CurrentSaveArchiveName = ArchiveName;
		SetPlayerInfo(OwnerETSA->PlayerInfo);
		return true;
	}

	return false;
}

bool UEveryThingGameInstance::SaveCurrentArchive() { return SaveArchiveWithName(CurrentSaveArchiveName); }

bool UEveryThingGameInstance::SaveArchiveWithName(const FString& ArchiveName)
{
	UEveryThingSaveArchive* OwnerETSA = Cast<UEveryThingSaveArchive>(UGameplayStatics::CreateSaveGameObject(UEveryThingSaveArchive::StaticClass()));;
	if (OwnerETSA)
	{
		OwnerETSA->PlayerInfo = PlayerInfo;
		if (UGameplayStatics::SaveGameToSlot(OwnerETSA, ArchiveName, 0))
		{
			CurrentSaveArchiveName = ArchiveName;
			return true;
		}
	}

	return false;
}


bool UEveryThingGameInstance::LoadArchivesList()
{
	UEveryThingSaveArchivesList* OwnerETSAL = Cast<UEveryThingSaveArchivesList>(UGameplayStatics::LoadGameFromSlot(TEXT("EveryThingArchivesList"), 0));
	if (OwnerETSAL)
	{
		CurrentArchivesList = OwnerETSAL->ArchivesList;
		return true;
	}
	return false;
}

void UEveryThingGameInstance::UpdateArchivesList()
{
	UEveryThingSaveArchivesList* OwnerETSAL = Cast<UEveryThingSaveArchivesList>(UGameplayStatics::CreateSaveGameObject(UEveryThingSaveArchivesList::StaticClass()));;
	if (OwnerETSAL)
	{
		OwnerETSAL->ArchivesList = CurrentArchivesList;
		bool bSuccess = UGameplayStatics::SaveGameToSlot(OwnerETSAL, TEXT("EveryThingArchivesList"), 0);
		
		check(bSuccess);
	}
}