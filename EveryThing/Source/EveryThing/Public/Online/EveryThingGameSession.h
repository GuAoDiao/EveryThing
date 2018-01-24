// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Online.h"
#include "OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/GameSession.h"
#include "EveryThingGameSession.generated.h"


struct FEveryThingGameSessionParams
{
public:
	// name of session settings are stored with
	FName SessionName;
	// LAN Match
	bool bIsLAN;
	// Presence enabled session
	bool bIsPresence;
	// id of player initiating lobby
	const FUniqueNetId* UserId;
	// current search result choice to join
	int32 BestSessionIndex;

	FEveryThingGameSessionParams() : SessionName(NAME_None), bIsLAN(false), bIsPresence(false), BestSessionIndex(0)
	{

	}
};

class FEveryThingOnlineSessionSettings : public FOnlineSessionSettings
{
public:
	FEveryThingOnlineSessionSettings(bool bIsLAN = false, bool bIsPresence = false, int32 MaxPlayersNum = 4);
	virtual ~FEveryThingOnlineSessionSettings() {}
};

class FEveryThingOnlineSearchSettings : public FOnlineSessionSearch
{
public:
	FEveryThingOnlineSearchSettings(bool bSearchingLAN = false, bool bSearchingPresence = false);
	virtual ~FEveryThingOnlineSearchSettings() {}
};

class FEveryThingOnlineSearchSettingsEmptyDedicated : public FEveryThingOnlineSearchSettings
{
public:
	FEveryThingOnlineSearchSettingsEmptyDedicated(bool bSearchingLAN = false, bool bSearchingPresence = false);
	virtual ~FEveryThingOnlineSearchSettingsEmptyDedicated() {}
};

/**
 * 
 */
UCLASS()
class EVERYTHING_API AEveryThingGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:
	AEveryThingGameSession();

protected:
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	
	// Transient properties of a session during game creation/matchmaking
	FEveryThingGameSessionParams CurrentSessionParams;

	TSharedPtr<FEveryThingOnlineSessionSettings> HostSettings;
	TSharedPtr<FEveryThingOnlineSearchSettings> SearchSettings;

	// rest the variables the are keeping track of session join attempts
	void ResetBestSessionVars();
	// choose the best session from a list of search results based on game criteria
	void ChooseBestSession();
	// entry point for matchmaking after search results are returned
	void StartMatchingmaking();
	// return point after each attempt to join a search result
	void ContinueMatchmaking();
	// delegate triggered when no more search results are available
	void OnNoMatchesAvaiable();
	// called when this instance is starting up as a dedicated server
	virtual void RegisterServer() override;

	DECLARE_EVENT_TwoParams(AEveryThingGameSession, FOnCreatePresenceSessionComplete, FName /*SessionName*/, bool /*bWasSuccessful*/);
	FOnCreatePresenceSessionComplete CreatePresenceSessionCompleteEvent;
	DECLARE_EVENT_OneParam(AEveryThingGameSession, FOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type /*Result*/)
	FOnJoinSessionComplete JoinSessionCompleteEvent;
	DECLARE_EVENT_OneParam(AEveryThingGameSession, FOnFindSessionsComplete, bool /*bWasSuccessful*/);
	FOnFindSessionsComplete FindSessionsCompleteEvent;
public:
	static const int32 DEFAULT_PLAYERS_NUM = 8;

	bool HostSession(const FUniqueNetId& UserId, FName SessionName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	void FindSessions(const FUniqueNetId& UserId, FName SessionName, bool bIsLAN, bool bIsPresence);
	bool JoinSession(const FUniqueNetId& UserId, FName SessionName, int32 SessionIndexInSearResults);
	bool JoinSession(const FUniqueNetId& UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

protected:
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);


public:
	// return true if any Online _async work is in progress, false otherwise
	bool IsBusy() const;

	EOnlineAsyncTaskState::Type GetSearchResultStatus(int32& SearchResultIndex, int32& NumSearchResults);

	const TArray<FOnlineSessionSearchResult>& GetSearchResults() const;

	FOnCreatePresenceSessionComplete& OnCreatePresenceSessionComplete() { return CreatePresenceSessionCompleteEvent; }
	FOnJoinSessionComplete& OnJoinSessionComplete() { return JoinSessionCompleteEvent; }
	FOnFindSessionsComplete& OnFindSessionsComplete() { return FindSessionsCompleteEvent; }

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;

	bool TravelToSession(int32 ControllerId, FName SessionName);

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
};
