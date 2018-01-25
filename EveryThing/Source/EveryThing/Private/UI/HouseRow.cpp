// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseRow.h"

#include "OnlineSessionSettings.h"

#include "EveryThingGameInstance.h"

void UHouseRow::InitializeUI(FOnlineSessionSearchResult& InSearchResult)
{
	SearchResult = &InSearchResult;

	UpdateDisplay();
}


void UHouseRow::UpdateDisplay()
{
	if (SearchResult)
	{
		const FOnlineSessionSettings& Settings = SearchResult->Session.SessionSettings;

		FString GameType, MapName;

		Settings.Get<FString>(SETTING_GAMEMODE, GameType);
		Settings.Get<FString>(SETTING_MAPNAME, MapName);
		int32 MaxPlayersNum = Settings.NumPublicConnections;
		int32 Pin = SearchResult->PingInMs;
		int32 CurrentPlayersNum = 0;

		InitializeDisplay(GameType, MapName, CurrentPlayersNum, MaxPlayersNum, Pin);
	}
}


void UHouseRow::JoinHouse()
{
	if (SearchResult)
	{
		APlayerController* OwnerPC = GetOwningPlayer();
		UEveryThingGameInstance* OwnerETGI = OwnerPC ? Cast<UEveryThingGameInstance>(OwnerPC->GetGameInstance()) : nullptr;
		if (OwnerETGI)
		{
			OwnerETGI->JoinGame(*SearchResult);
		}
	}
}