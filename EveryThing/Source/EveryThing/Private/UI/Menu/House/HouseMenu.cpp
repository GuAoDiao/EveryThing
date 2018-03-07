// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseMenu.h"

#include "UI/Menu/EveryThingMenuHUD.h"

void UHouseMenu::CreateHouse()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::HouseCreate); }
}

void UHouseMenu::HouseList()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::HouseList); }
}

void UHouseMenu::Backup()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MasterInterface); }
}