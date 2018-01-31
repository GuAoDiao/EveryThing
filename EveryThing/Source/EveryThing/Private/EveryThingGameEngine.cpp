// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameEngine.h"

#include "EveryThingGameUserSettings.h"

UEveryThingGameEngine::UEveryThingGameEngine()
{
	GameUserSettingsClass = UEveryThingGameUserSettings::StaticClass();
}