// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballSkin.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

#include "EveryThingAssetManager.h"

void FFootballSkin::LoadGamePawnSkin()
{
	if (OwnerStaticMeshComp)
	{
		UEveryThingAssetManager* AssetManager = UEveryThingAssetManager::GetAssetManagerInstance();

		UMaterialInstanceConstant* FootballMaterialBlack = AssetManager->GetMaterialFromName(TEXT("FootballBlack"));
		UMaterialInstanceConstant* FootballMaterialWhite = AssetManager->GetMaterialFromName(TEXT("FootballWhite"));

		if (FootballMaterialBlack && FootballMaterialWhite)
		{
			OwnerStaticMeshComp->SetMaterial(0, FootballMaterialBlack);
			OwnerStaticMeshComp->SetMaterial(1, FootballMaterialWhite);
		}
	}
}