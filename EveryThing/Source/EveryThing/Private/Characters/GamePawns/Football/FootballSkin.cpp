// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballSkin.h"

#include "EveryThingAssetManager.h"

FFootballSkin::FFootballSkin(UStaticMeshComponent* StaticMeshComp) : FGamePawnSkin(StaticMeshComp)
{
	MaterialNames.Add(TEXT("FootballBlack"));
	MaterialNames.Add(TEXT("FootballWhite"));

	LoadAllGameSkinMaterial();
}