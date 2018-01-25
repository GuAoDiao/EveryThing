// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseRow.h"

#include "OnlineSessionSettings.h"

#include "EveryThingGameInstance.h"
#include "UI/EveryThingMenuHUD.h"

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

		FString GameType, MapName, HouseName;

		Settings.Get<FString>(SETTING_GAMEMODE, GameType);
		Settings.Get<FString>(SETTING_MAPNAME, MapName);
		Settings.Get<FString>(FName("HouseName"), HouseName);
		

		int32 MaxPlayersNum = Settings.NumPublicConnections;
		int32 Pin = SearchResult->PingInMs;
		int32 CurrentPlayersNum = 0;

		InitializeDisplay(HouseName, GameType, MapName, CurrentPlayersNum, MaxPlayersNum, Pin);
	}
}


void UHouseRow::JoinHouse()
{
	APlayerController* OwnerPC = GetOwningPlayer();
	if (OwnerPC && SearchResult)
	{
		AEveryThingMenuHUD* OwnerMenuHUD = Cast<AEveryThingMenuHUD>(OwnerPC->GetHUD());
		if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EGameUIState::LoadingScreen); }

		UEveryThingGameInstance* OwnerETGI = Cast<UEveryThingGameInstance>(OwnerPC->GetGameInstance());
		if (OwnerETGI) { OwnerETGI->JoinGame(*SearchResult); }
	}
}