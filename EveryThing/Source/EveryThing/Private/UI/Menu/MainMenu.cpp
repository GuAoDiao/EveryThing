// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "EveryThingGameInstance.h"
#include "UI/Menu/EveryThingMenuHUD.h"


void UMainMenu::CreateArchive()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::CreateArchive); }
}

void UMainMenu::ArchiveList()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::ArchiveList); }
}


void UMainMenu::ExitGame()
{
	UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
	if (OwnerETGI) { OwnerETGI->ExitGameApplication(); }
}