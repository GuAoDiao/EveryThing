// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "EveryThingGameInstance.h"
#include "UI/EveryThingMenuHUD.h"

void UMainMenu::OnCreateHouse()
{
	APlayerController* OwnerPC = GetOwningPlayer();
	AEveryThingMenuHUD* OwnerMenuHUD = OwnerPC ? Cast<AEveryThingMenuHUD>(OwnerPC->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EGameUIState::HouseCreate); }
}

void UMainMenu::OnHouseList()
{

	APlayerController* OwnerPC = GetOwningPlayer();
	AEveryThingMenuHUD* OwnerMenuHUD = OwnerPC ? Cast<AEveryThingMenuHUD>(OwnerPC->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EGameUIState::HouseList); }
}

void UMainMenu::OnConfig()
{
	APlayerController* OwnerPC = GetOwningPlayer();
	AEveryThingMenuHUD* OwnerMenuHUD = OwnerPC ? Cast<AEveryThingMenuHUD>(OwnerPC->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EGameUIState::Config); }
}

void UMainMenu::OnExitGame()
{
	APlayerController* OwnerPC = GetOwningPlayer();
	UEveryThingGameInstance* OwnerETGI = OwnerPC ? Cast<UEveryThingGameInstance>(OwnerPC->GetGameInstance()) : nullptr;
	if (OwnerETGI)
	{
		OwnerETGI->ExitGame();
	}
}