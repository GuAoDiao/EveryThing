// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameUserSettings.h"


UEveryThingGameUserSettings::UEveryThingGameUserSettings()
{
	SetToDefaults();


	GEngine;
}

void UEveryThingGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	GraphicsQuality = 1;
	bIsLANMatch = true;
	bIsDedicatedServer = false;
}

void UEveryThingGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);

	if (GraphicsQuality == 1 || GraphicsQuality == 2 || GraphicsQuality == 3) { ScalabilityQuality.SetFromSingleQualityLevel(GraphicsQuality); }
}
