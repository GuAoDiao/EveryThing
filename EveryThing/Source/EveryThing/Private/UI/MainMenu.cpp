// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "EveryThingGameInstance.h"

void UMainMenu::HostGame()
{
	UWorld* World = GetWorld();
	UEveryThingGameInstance* OwnerGameInstance =World ? World->GetGameInstance<UEveryThingGameInstance>() : nullptr;
	if (OwnerGameInstance) { OwnerGameInstance->HostGame(); }
}

void UMainMenu::FindGame()
{

}

void UMainMenu::ExitGame()
{

}

