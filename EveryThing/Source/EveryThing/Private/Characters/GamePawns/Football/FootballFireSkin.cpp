// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballFireSkin.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

#include "EveryThingAssetManager.h"

void FFootballFireSkin::LoadGamePawnSkin()
{
	if (OwnerStaticMeshComp)
	{
		UEveryThingAssetManager* AssetManager = UEveryThingAssetManager::GetAssetManagerInstance();

		UMaterialInstanceConstant* FootballMaterialBlack = AssetManager->GetMaterialFromName(TEXT("FootballBlack_Fire"));
		UMaterialInstanceConstant* FootballMaterialWhite = AssetManager->GetMaterialFromName(TEXT("FootballWhite_Fire"));

		if (FootballMaterialBlack && FootballMaterialWhite)
		{
			OwnerStaticMeshComp->SetMaterial(0, FootballMaterialBlack);
			OwnerStaticMeshComp->SetMaterial(1, FootballMaterialWhite);
		}
	}
}