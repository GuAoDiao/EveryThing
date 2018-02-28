// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseList.h"

#include "GameFramework/PlayerController.h"

#include "EveryThingGameInstance.h"
#include "EveryThingAssetManager.h"
#include "UI/Menu/EveryThingMenuHUD.h"
#include "UI/Menu/HouseRow.h"


void UHouseList::FindAllHouseList(bool bIsLAN, bool bIsPresence)
{
	APlayerController* OwnerPC = GetOwningPlayer();
	if (OwnerPC)
	{
		AEveryThingMenuHUD* OwnerMenuHUD = Cast<AEveryThingMenuHUD>(OwnerPC->GetHUD());
		if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::LoadingScreen); }

		UEveryThingGameInstance* OwnerETGI = Cast<UEveryThingGameInstance>(OwnerPC->GetGameInstance());
		if (OwnerETGI) { OwnerETGI->FindHoustList(bIsLAN, bIsPresence); }
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

void UHouseList::BackUp()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD && OwnerMenuHUD->IsTargetGameUIState(EMenuUIState::HouseList))
	{
		OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::HouseMenu);		
	}
}