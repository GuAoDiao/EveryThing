// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMenu.h"

#include "Engine/World.h"

#include "EveryThingGameInstance.h"

void UGameMenu::Confing()
{

}

void UGameMenu::ExitGame()
{
	UEveryThingGameInstance* OwnerGameInstance = GetWorld() ? GetWorld()->GetGameInstance<UEveryThingGameInstance>() : nullptr;
	if (OwnerGameInstance) { OwnerGameInstance->ExitGame(); }
}