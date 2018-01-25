// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseCreate.h"

#include "EveryThingTypes.h"
#include "EveryThingAssetManager.h"
#include "EveryThingGameInstance.h"
#include "UI/EveryThingMenuHUD.h"

UHouseCreate::UHouseCreate(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MapsInfoDataTable = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName("MapsInfo");
	if (MapsInfoDataTable)
	{
		TArray<FMapInfo*> MapsInfoInDataTable;
		MapsInfoDataTable->GetAllRows<FMapInfo>(TEXT("look up all maps info"), MapsInfoInDataTable);
		for (FMapInfo* MapInfo : MapsInfoInDataTable)
		{
			MapsType.AddUnique(MapInfo->MapType);
		}
	}
}

void UHouseCreate::HostGame(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum)
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EGameUIState::LoadingScreen); }

	UEveryThingGameInstance* OwnerGameInstance = GetWorld() ? GetWorld()->GetGameInstance<UEveryThingGameInstance>() : nullptr;
	if (OwnerGameInstance) { OwnerGameInstance->HostGame(HouseName, GameType, MapName, bIsLAN, bIsPresence, MaxPlayersNum); }
}


void UHouseCreate::BackUp()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EGameUIState::MainMenu); }
}

TArray<FString> UHouseCreate::GetAllMaps(const FString& MapType) const
{
	TArray<FString> AllMaps;

	if (MapsInfoDataTable)
	{
		TArray<FMapInfo*> MapsInfoInDataTable;
		MapsInfoDataTable->GetAllRows<FMapInfo>(TEXT("look up all maps info"), MapsInfoInDataTable);
		for (FMapInfo* MapInfo : MapsInfoInDataTable)
		{
			if (MapInfo->MapType == MapType) { AllMaps.AddUnique(MapInfo->MapName); }
		}
	}

	return AllMaps;
}