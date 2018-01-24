// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameMode_Menu.h"

#include "EveryThingGameInstance.h"

#include "Online/EveryThingGameSession.h"
#include "UI/EveryThingMenuHUD.h"


AEveryThingGameMode_Menu::AEveryThingGameMode_Menu()
{
	GameSessionClass = AEveryThingGameSession::StaticClass();
	HUDClass = AEveryThingMenuHUD::StaticClass();
}