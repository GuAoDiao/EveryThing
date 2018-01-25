// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameSession.h"

#include "Engine/World.h"
#include "OnlineSubsystemSessionSettings.h"

#include "EveryThingGameInstance.h"
#include "UI/EveryThingMenuHUD.h"


DECLARE_LOG_CATEGORY_CLASS(EveryThingOnline, Log, All);

namespace
{
	const FString CustomMatchKeyboard("EveryThing");
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
	// PingBucketSize = 50;
	if (bSearchingPresence) { QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); }
}

FEveryThingOnlineSearchSettingsEmptyDedicated::FEveryThingOnlineSearchSettingsEmptyDedicated(bool bSearchingLAN, bool bSearchingPresence) : FEveryThingOnlineSearchSettings(bSearchingLAN, bSearchingPresence)
{
	// QuerySettings.Set(SEARCH_DEDICATED_ONLY, true, EOnlineComparisonOp::Equals);
	// QuerySettings.Set(SEARCH_EMPTY_SERVERS_ONLY, true, EOnlineComparisonOp::Equals);
}


AEveryThingGameSession::AEveryThingGameSession()
{
	// if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnCreateSessionComplete);
		OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnStartOnlineGameComplete);
		OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnFindSessionsComplete);
		OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnJoinSessionComplete);
		OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnDestroySessionComplete);
	}
}


bool AEveryThingGameSession::HostSession(const FUniqueNetId& UserId, FName InSessionName, const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum)
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
				HostSettings->Set(SETTING_MAPNAME, MapName, EOnlineDataAdvertisementType::ViaOnlineService);
				HostSettings->Set(FName("HouseName"), HouseName, EOnlineDataAdvertisementType::ViaOnlineService);

				HostSettings->Set(SETTING_MATCHING_TIMEOUT, 120.f, EOnlineDataAdvertisementType::ViaOnlineService);
				HostSettings->Set(SETTING_MATCHING_HOPPER, FString("TeamDeathmatch"), EOnlineDataAdvertisementType::DontAdvertise);
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

void AEveryThingGameSession::FindSessions(const FUniqueNetId& UserId, bool bIsLAN, bool bIsPresence)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem)
	{
		OnFindSessionsComplete(false);
		return;
	}

	CurrentSessionParams.bIsLAN = bIsLAN;
	CurrentSessionParams.bIsPresence = bIsPresence;
	CurrentSessionParams.UserId = &UserId;

	IOnlineSessionPtr Sessions = Subsystem->GetSessionInterface();

	if (Sessions.IsValid() && CurrentSessionParams.UserId)
	{
		SearchSettings = MakeShareable(new FEveryThingOnlineSearchSettings(bIsLAN, bIsPresence));
		if (SearchSettings.IsValid())
		{
#if !PLATFORM_SWITCH
			SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS, CustomMatchKeyboard, EOnlineComparisonOp::Equals);
#endif

			// Sessions->CancelFindSessions();
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			Sessions->FindSessions(*CurrentSessionParams.UserId, SearchSettings.ToSharedRef());
		}
	}
}

bool AEveryThingGameSession::JoinSession(const FUniqueNetId& UserId, int32 SessionIndexInSearResults)
{
	bool bResult = false;

	if (SearchSettings.IsValid())
	{
		if (SessionIndexInSearResults >= 0 && SessionIndexInSearResults < SearchSettings->SearchResults.Num())
		{
			bResult = JoinSession(UserId, SearchSettings->SearchResults[SessionIndexInSearResults]);
		}
	}

	return bResult;
}

bool AEveryThingGameSession::JoinSession(const FUniqueNetId& UserId, const FOnlineSessionSearchResult& SearchResult)
{
	bool bResult = false;
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		CurrentSessionParams.SessionName = *SearchResult.Session.OwningUserName;
		OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
		bResult = Sessions->JoinSession(UserId, CurrentSessionParams.SessionName, SearchResult);
	}

	return bResult;
}



void AEveryThingGameSession::OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(EveryThingOnline, Log, TEXT("-_- OnCreateSessionComplete %s bSuccess : %d"), *InSessionName.ToString(), bWasSuccessful);

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
	UE_LOG(EveryThingOnline, Log, TEXT("-_- OnStartOnlineGameComplete bSuccess : %d "), bWasSuccessful);

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{

		Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
	}


	if (bWasSuccessful)
	{
		UEveryThingGameInstance* OwnerETGI = Cast<UEveryThingGameInstance>(GetGameInstance());
		FOnlineSessionSettings* OwnerOnlineSessionSetting = Sessions->GetSessionSettings(InSessionName);

		if (OwnerETGI && OwnerOnlineSessionSetting)
		{
			FString GameType, MapName;
			OwnerOnlineSessionSetting->Get<FString>(SETTING_GAMEMODE, GameType);
			OwnerOnlineSessionSetting->Get<FString>(SETTING_MAPNAME, MapName);
			OwnerETGI->OpenGameLevel(GameType, MapName);
		}
	}
}

void AEveryThingGameSession::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(EveryThingOnline, Log, TEXT("-_- OnFindSessionsCOmplete bSuccess: %d"), bWasSuccessful);


	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

		if (SearchSettings.IsValid())
		{
			UE_LOG(EveryThingOnline, Log, TEXT("-_- Num Search Results: %d"), SearchSettings->SearchResults.Num());
			
			APlayerController* OwnerPC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
			AEveryThingMenuHUD* OwnerETMH = OwnerPC ? Cast<AEveryThingMenuHUD>(OwnerPC->GetHUD()) : nullptr;
			if(OwnerETMH){ OwnerETMH->UpdateHouseList(SearchSettings->SearchResults); }

			for (int32 SearchIndex = 0; SearchIndex < SearchSettings->SearchResults.Num(); ++SearchIndex)
			{
				const FOnlineSessionSearchResult& SearchResult = SearchSettings->SearchResults[SearchIndex];
				DumpSession(&SearchResult.Session);
			}
		}
	}
}

void AEveryThingGameSession::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	bool bWillTravel = false;

	UE_LOG(EveryThingOnline, Log, TEXT("-_- OnjoinSessionComplete %s bSuccess: %d"), *InSessionName.ToString(), static_cast<int32>(Result));

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

		// get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
		// this is something the blueprint node "JoinSession" does automatically!
		APlayerController* const OwnerPC = GetGameInstance() ? GetGameInstance()->GetFirstLocalPlayerController() : nullptr;

		// we need a FString to use ClientTravel and we can let the session interface construct such a String for
		// us by giving him the session name and an empty string. we want to do this, because every Online subsystem uses different TravelURLs
		FString TravelURL;

		if (OwnerPC && Sessions->GetResolvedConnectString(InSessionName, TravelURL))
		{
			// finally call the ClientTravel. if you want, you could print the TravelURL to see how it really looks like
			OwnerPC->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
		}
	}
}

void AEveryThingGameSession::OnDestroySessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(EveryThingOnline, Log, TEXT("-_- OnDestroySessionComplete %s bSuccess: %d"), *InSessionName.ToString(), bWasSuccessful);

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
		UE_LOG(EveryThingOnline, Log, TEXT("-_- Starting session %s on server"), *FName(NAME_GameSession).ToString());

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
		}

		// server is handled here
		UE_LOG(EveryThingOnline, Log, TEXT("-_- Ending session %s on server"), *FName(NAME_GameSession).ToString());

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
	UE_LOG(EveryThingOnline, Log, TEXT("Matchmaking complete, no sessions avaiable."));
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
			UE_LOG(EveryThingOnline, Warning, TEXT("Failed to join session %s"), *SessionName.ToString());
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