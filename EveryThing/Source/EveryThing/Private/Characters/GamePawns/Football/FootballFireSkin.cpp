// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballFireSkin.h"

#include "EveryThingAssetManager.h"

FFootballFireSkin::FFootballFireSkin(UStaticMeshComponent* StaticMeshComp) : FGamePawnSkin(StaticMeshComp)
{
	MaterialNames.Add(TEXT("FootballBlack_Fire"));
	MaterialNames.Add(TEXT("FootballWhite_Fire"));

	UEveryThingAssetManager::GetAssetManagerInstance()->NeededMaterialFromName(TEXT("FootballBlack_Fire"));
	UEveryThingAssetManager::GetAssetManagerInstance()->NeededMaterialFromName(TEXT("FootballWhite_Fire"));
}