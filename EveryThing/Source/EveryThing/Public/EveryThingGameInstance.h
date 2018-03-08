// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EveryThingTypes.h"
#include "EveryThingGameInstance.generated.h"

class UEveryThingSaveArchive;
class ULoadingMap;

/**
 * 
 */
UCLASS()
class EVERYTHING_API UEveryThingGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UEveryThingGameInstance();

	virtual void Init() override;

	//////////////////////////////////////////////////////////////////////////
	/// Level
public:
	void OpenMenuLevel();
	void OpenGameLevel(const FString& MapType, const FString& MapName);
	void ExitGameApplication();


	//////////////////////////////////////////////////////////////////////////
	/// Loading Map
public:
	void BeginLoadingMap(const FString& MapName);
	void EndLoadingMap(UWorld* LoadedWorld);


	UFUNCTION(BlueprintCallable)
	bool IsLoadingMapFinished() const;
private:
	class IGameMoviePlayer* LoadingMapMoviePlayer;
	UPROPERTY(Transient)
	ULoadingMap* LoadingMap;

	//////////////////////////////////////////////////////////////////////////
	/// Match
public:
	void HostGame(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum);
	void FindHoustList(bool bIsLAN, bool bIsPresence);
	void JoinGame(FOnlineSessionSearchResult& SessionResult);
	void JoinGame(int32 SearchResultIndex);
	void ExitGame();

private:
	class AEveryThingGameSession* GetGameSession();
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName MenuLevelName;




	//////////////////////////////////////////////////////////////////////////
	/// Player Info
public:
	bool LoadArchivesList();
	void UpdateArchivesList();
	const TArray<FString>& GetArchivesList() { return CurrentArchivesList; }

	bool CreateArchive(const FString& ArchiveName, const FString& PlayerName);

	bool LoadArchiveFromName(const FString& ArchiveName);
	bool SaveArchiveWithName(const FString& ArchiveName);
	bool SaveCurrentArchive();

	void SetPlayerInfo(const FPlayerInfo& InPlayerInfo) { PlayerInfo = InPlayerInfo; SaveCurrentArchive(); }
	UFUNCTION(BlueprintPure)
	FPlayerInfo& GetPlayerInfo() { return PlayerInfo; }
	UFUNCTION(BlueprintPure)
	const FString& GetArchiveName() const { return CurrentSaveArchiveName; }
	
private:
	TArray<FString> CurrentArchivesList;

	FPlayerInfo PlayerInfo;
	FString CurrentSaveArchiveName;
};
