// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseList.h"

#include "GameFramework/PlayerController.h"

#include "EveryThingGameInstance.h"
#include "EveryThingAssetManager.h"
#include "UI/HouseRow.h"
#include "UI/EveryThingMenuHUD.h"


void UHouseList::FindAllHouseList(bool bIsLAN, bool bIsPresence)
{
	APlayerController* OwnerPC = GetOwningPlayer();
	UEveryThingGameInstance* OwnerETGI = OwnerPC ? Cast<UEveryThingGameInstance>(OwnerPC->GetGameInstance()) : nullptr;
	if (OwnerETGI)
	{
		OwnerETGI->FindHoustList(bIsLAN, bIsPresence);
	}
}

void UHouseList::UpdateHouseList(TArray<FOnlineSessionSearchResult>& SearchResults)
{
	TSubclassOf<UUserWidget> HouseRowClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName(TEXT("HouseRow"));
	if (HouseRowClass)
	{
		RemoveAllHouseRows();

		for (int32 SearchIndex = 0; SearchIndex < SearchResults.Num(); ++SearchIndex)
		{
			UHouseRow* HouseRow = CreateWidget<UHouseRow>(GetOwningPlayer(), HouseRowClass);
			if (HouseRow)
			{
				HouseRow->InitializeUI(SearchResults[SearchIndex]);
				AddHouseRow(HouseRow);
			}

		}
	}
}

void UHouseList::Back()
{
	APlayerController* OwnerPC = GetOwningPlayer();
	AEveryThingMenuHUD* OwnerMenuHUD = OwnerPC ? Cast<AEveryThingMenuHUD>(OwnerPC->GetHUD()) : nullptr;
	if (OwnerMenuHUD)
	{
		OwnerMenuHUD->ToggleToNewGameUIState(EGameUIState::MainMenu);
	}
}