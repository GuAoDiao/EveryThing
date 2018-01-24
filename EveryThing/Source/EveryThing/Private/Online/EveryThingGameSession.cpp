// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameSession.h"

#include "Engine/World.h"
#include "OnlineSubsystemSessionSettings.h"

#include "Characters/Controlllers/PlayerPawnController.h"

namespace
{
	const FString CustomMatchKeyboard("Custom");
}

FEveryThingOnlineSessionSettings::FEveryThingOnlineSessionSettings(bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum)
{
	NumPublicConnections  = MaxPlayersNum > 0 ? MaxPlayersNum : 0;
	NumPrivateConnections = 0;
	bIsLANMatch = bIsLAN;
	bShouldAdvertise = true;
	bAllowJoinInProgress = true;
	bAllowInvites = true;
	bUsesPresence = bIsPresence;
	bAllowJoinViaPresence = true;
	bAllowJoinViaPresenceFriendsOnly = false;
}

FEveryThingOnlineSearchSettings::FEveryThingOnlineSearchSettings(bool bSearchingLAN, bool bSearchingPresence)
{
	bIsLanQuery = bSearchingLAN;
	MaxSearchResults = 10;
	PingBucketSize = 50;
	if (bSearchingPresence) { QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); }
}

FEveryThingOnlineSearchSettingsEmptyDedicated::FEveryThingOnlineSearchSettingsEmptyDedicated(bool bSearchingLAN, bool bSearchingPresence) : FEveryThingOnlineSearchSettings(bSearchingLAN, bSearchingPresence)
{
	QuerySettings.Set(SEARCH_DEDICATED_ONLY, true, EOnlineComparisonOp::Equals);
	QuerySettings.Set(SEARCH_EMPTY_SERVERS_ONLY, true, EOnlineComparisonOp::Equals);
}


AEveryThingGameSession::AEveryThingGameSession()
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnCreateSessionComplete);
		OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnStartOnlineGameComplete);
		OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnFindSessionsComplete);
		OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnJoinSessionComplete);
		OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnDestroySessionComplete);
	}
}


bool AEveryThingGameSession::HostSession(const FUniqueNetId& UserId, FName InSessionName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		CurrentSessionParams.SessionName = InSessionName;
		CurrentSessionParams.bIsLAN = bIsLAN;
		CurrentSessionParams.bIsPresence = bIsPresence;
		CurrentSessionParams.UserId = &UserId;

		MaxPlayers = MaxPlayersNum;

		IOnlineSessionPtr Sessions = Subsystem->GetSessionInterface();
		if (Sessions.IsValid() && CurrentSessionParams.UserId)
		{
			HostSettings = MakeShareable(new FEveryThingOnlineSessionSettings(bIsLAN, bIsPresence, MaxPlayers));
			if (HostSettings.IsValid())
			{

				HostSettings->Set(SETTING_GAMEMODE, GameType, EOnlineDataAdvertisementType::ViaOnlineService);
				HostSettings->Set(SETTING_GAMEMODE, MapName, EOnlineDataAdvertisementType::ViaOnlineService);
				HostSettings->Set(SETTING_MATCHING_HOPPER, FString("TeamDeathmatch"), EOnlineDataAdvertisementType::DontAdvertise);
				HostSettings->Set(SETTING_MATCHING_TIMEOUT, 120.f, EOnlineDataAdvertisementType::ViaOnlineService);
				HostSettings->Set(SETTING_SESSION_TEMPLATE_NAME, FString("GameSession"), EOnlineDataAdvertisementType::DontAdvertise);

#if !PLATFORM_SWITCH
				// on switch, we don't have room for this in the session data(and it's not used anyway when searching), so there's no need to add it
				HostSettings->Set(SEARCH_KEYWORDS, CustomMatchKeyboard, EOnlineDataAdvertisementType::ViaOnlineService);
#endif
				OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
				return Sessions->CreateSession(*CurrentSessionParams.UserId, CurrentSessionParams.SessionName, *HostSettings);
			}
		}
	}
#if !UE_BUILD_SHIPPING
	else
	{
		// hack work flow in development
		OnCreatePresenceSessionComplete().Broadcast(NAME_GameSession, true);
	}
#endif

	return false;
}

void AEveryThingGameSession::FindSessions(const FUniqueNetId& UserId, FName InSessionName, bool bIsLAN, bool bIsPresence)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem)
	{
		OnFindSessionsComplete(false);
		return;
	}

	CurrentSessionParams.SessionName = InSessionName;
	CurrentSessionParams.bIsLAN = bIsLAN;
	CurrentSessionParams.bIsPresence = bIsPresence;
	CurrentSessionParams.UserId = &UserId;

	IOnlineSessionPtr Sessions = Subsystem->GetSessionInterface();
	if (Sessions.IsValid() && CurrentSessionParams.UserId)
	{
		SearchSettings = MakeShareable(new FEveryThingOnlineSearchSettings(bIsLAN, bIsPresence));
		if (SearchSettings.IsValid())
		{
			SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS, CustomMatchKeyboard, EOnlineComparisonOp::Equals);

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SearchSettings.ToSharedRef();

			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			Sessions->FindSessions(*CurrentSessionParams.UserId, SearchSettingsRef);
		}
	}
}

bool AEveryThingGameSession::JoinSession(const FUniqueNetId& UserId, FName InSessionName, int32 SessionIndexInSearResults)
{
	bool bResult = false;

	if (SearchSettings.IsValid())
	{
		if (SessionIndexInSearResults >= 0 && SessionIndexInSearResults < SearchSettings->SearchResults.Num())
		{
			bResult = JoinSession(UserId, InSessionName, SearchSettings->SearchResults[SessionIndexInSearResults]);
		}
	}

	return bResult;
}

bool AEveryThingGameSession::JoinSession(const FUniqueNetId& UserId, FName InSessionName, const FOnlineSessionSearchResult& SearchResult)
{
	bool bResult = false;
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
		bResult = Sessions->JoinSession(UserId, InSessionName, SearchResult);
	}

	return bResult;
}



void AEveryThingGameSession::OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogOnlineGame, Verbose, TEXT("-_- OnCreateSessionComplete %s bSuccess : %d"), *InSessionName.ToString(), bWasSuccessful);

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);

		if (bWasSuccessful)
		{
			OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
			
			Sessions->StartSession(InSessionName);
		}
	}
	OnCreatePresenceSessionComplete().Broadcast(InSessionName, bWasSuccessful);
}

void AEveryThingGameSession::OnStartOnlineGameComplete(FName InSessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
	}

	UWorld* World = GetWorld();
	if (bWasSuccessful && World)
	{
		// tell non-local players to start Online game
		for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerPawnController* PlayerPPC = Cast<APlayerPawnController>(*It);
			if (PlayerPPC && PlayerPPC->IsLocalPlayerController())
			{
				PlayerPPC->ClientStartOnlineGame();
			}
		}
	}
}

void AEveryThingGameSession::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogOnlineGame, Verbose, TEXT("-_- OnFindSessionsCOmplete bSuccess: %d"), bWasSuccessful);


	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

		if (SearchSettings.IsValid())
		{
			UE_LOG(LogOnlineGame, Verbose, TEXT("-_- Num Search Results: %d"), SearchSettings->SearchResults.Num());

			for (int32 SearchIndex = 0; SearchIndex < SearchSettings->SearchResults.Num(); ++SearchIndex)
			{
				const FOnlineSessionSearchResult& SearchResult = SearchSettings->SearchResults[SearchIndex];
				DumpSession(&SearchResult.Session);
			}
		}

		OnFindSessionsComplete().Broadcast(bWasSuccessful);
	}
}

void AEveryThingGameSession::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	bool bWillTravel = false;

	UE_LOG(LogOnlineGame, Verbose, TEXT("-_- OnjoinSessionComplete %s bSuccess: $d"), *InSessionName.ToString(), static_cast<int32>(Result));

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	}

	OnJoinSessionComplete().Broadcast(Result);
}

void AEveryThingGameSession::OnDestroySessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogOnlineGame, Verbose, TEXT("-_- OnDestroySessionComplete %s bSuccess: %d"), *InSessionName.ToString(), bWasSuccessful);

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		HostSettings = nullptr;
	}
}


EOnlineAsyncTaskState::Type AEveryThingGameSession::GetSearchResultStatus(int32& SearchResultIndex, int32& NumSearchResults)
{
	SearchResultIndex = -1;
	NumSearchResults = -1;
	if (SearchSettings.IsValid())
	{
		if (SearchSettings.IsValid())
		{
			SearchResultIndex = CurrentSessionParams.BestSessionIndex;
			NumSearchResults = SearchSettings->SearchResults.Num();
		}
		return SearchSettings->SearchState;
	}

	return EOnlineAsyncTaskState::NotStarted;
}

const TArray<FOnlineSessionSearchResult>& AEveryThingGameSession::GetSearchResults() const
{
	check(SearchSettings.IsValid())
	
	return SearchSettings->SearchResults;
}

void AEveryThingGameSession::HandleMatchHasStarted()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		UE_LOG(LogOnlineGame, Log, TEXT("-_- Starting session %s on server"), *FName(NAME_GameSession).ToString());

		OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
		Sessions->StartSession(NAME_GameSession);
	}
}

void AEveryThingGameSession::HandleMatchHasEnded()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		UWorld* World = GetWorld();

		if (World)
		{
			// tell non-local players to end Online game
			for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
			{
				APlayerPawnController* PlayerPPC = Cast<APlayerPawnController>(*It);
				if (PlayerPPC && PlayerPPC->IsLocalPlayerController())
				{
					PlayerPPC->ClientEndOnlineGame();
				}
			}
		}

		// server is handled here
		UE_LOG(LogOnlineGame, Log, TEXT("-_- Ending session %s on server"), *FName(NAME_GameSession).ToString());

		Sessions->EndSession(NAME_GameSession);
	}
}


bool AEveryThingGameSession::IsBusy() const
{
	if (HostSettings.IsValid() || SearchSettings.IsValid())
	{
		return true;
	}

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		EOnlineSessionState::Type GameSessionState = Sessions->GetSessionState(NAME_GameSession);
		EOnlineSessionState::Type PartySessionState = Sessions->GetSessionState(NAME_PartySession);
		if (GameSessionState != EOnlineSessionState::NoSession || PartySessionState != EOnlineSessionState::NoSession)
		{
			return true;
		}
	}

	return false;
}

void AEveryThingGameSession::ResetBestSessionVars()
{
	CurrentSessionParams.BestSessionIndex = -1;
}

void AEveryThingGameSession::ChooseBestSession()
{
	// Start searching from where we left off
	if (SearchSettings.IsValid())
	{
		for (int32 SessionIndex = CurrentSessionParams.BestSessionIndex + 1; SessionIndex < SearchSettings->SearchResults.Num(); ++SessionIndex)
		{
			// found the match  that we want
			CurrentSessionParams.BestSessionIndex = SessionIndex;
			return;
		}
	}
	CurrentSessionParams.BestSessionIndex = -1;
}

void AEveryThingGameSession::StartMatchingmaking()
{
	ResetBestSessionVars();
	ContinueMatchmaking();
}

void AEveryThingGameSession::ContinueMatchmaking()
{
	ChooseBestSession();
	if (SearchSettings.IsValid() && CurrentSessionParams.BestSessionIndex > 0 && CurrentSessionParams.BestSessionIndex < SearchSettings->SearchResults.Num())
	{
		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
		if (Sessions.IsValid() && CurrentSessionParams.UserId)
		{
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			Sessions->JoinSession(*CurrentSessionParams.UserId, CurrentSessionParams.SessionName, SearchSettings->SearchResults[CurrentSessionParams.BestSessionIndex]);
		}
	}
	else
	{
		OnNoMatchesAvaiable();
	}
}

void AEveryThingGameSession::OnNoMatchesAvaiable()
{
	UE_LOG(LogOnlineGame, Verbose, TEXT("Matchmaking complete, no sessions avaiable."));
	SearchSettings = nullptr;
}

bool AEveryThingGameSession::TravelToSession(int32 ControllerId, FName InSessionName)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr Sessions = Subsystem->GetSessionInterface();
		FString URL;
		if (Sessions.IsValid() && Sessions->GetResolvedConnectString(InSessionName, URL))
		{
			UWorld* World = GetWorld();
			if (World)
			{
				int32 Index = 0;
				APlayerController* LocalPlayerController = nullptr;
				for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
				{
					if (Index == ControllerId)
					{
						LocalPlayerController = It->Get();
					}
					Index++;
				}

				if (LocalPlayerController)
				{
					LocalPlayerController->ClientTravel(URL, TRAVEL_Absolute);
					return true;
				}
			}
		}
		else
		{
			UE_LOG(LogOnlineGame, Warning, TEXT("Failed to join session %s"), *SessionName.ToString());
		}
	}
#if !UE_BUILD_SHIPPING
	else
	{
		UWorld* World = GetWorld();
		if (World)
		{
			int32 Index = 0;
			APlayerController* LocalPlayerController = nullptr;
			for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
			{
				if (Index == ControllerId)
				{
					LocalPlayerController = It->Get();
				}
				Index++;
			}

			if (LocalPlayerController)
			{
				FString LocalURL(TEXT("127.0.0.1"));
				LocalPlayerController->ClientTravel(LocalURL, TRAVEL_Absolute);
				return true;
			}
		}
	}
#endif

	return false;
}

void AEveryThingGameSession::RegisterServer()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		TSharedPtr<FEveryThingOnlineSessionSettings> ShooterHostSettings = MakeShareable(new FEveryThingOnlineSessionSettings(false, false, 16));
		if (ShooterHostSettings.IsValid())
		{
			ShooterHostSettings->Set(SETTING_MATCHING_HOPPER, FString("TeamDeathmatch"), EOnlineDataAdvertisementType::DontAdvertise);
			ShooterHostSettings->Set(SETTING_MATCHING_TIMEOUT, 120.f, EOnlineDataAdvertisementType::ViaOnlineService);
			ShooterHostSettings->Set(SETTING_SESSION_TEMPLATE_NAME, FString("GameSession"), EOnlineDataAdvertisementType::DontAdvertise);
			UWorld* World = GetWorld();
			if (World) { ShooterHostSettings->Set(SETTING_MAPNAME, World->GetMapName(), EOnlineDataAdvertisementType::ViaOnlineService); }

			ShooterHostSettings->bAllowInvites = true;
			ShooterHostSettings->bIsDedicated = true;

			HostSettings = ShooterHostSettings;
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			Sessions->CreateSession(0, NAME_GameSession, *HostSettings);
		}
	}
}