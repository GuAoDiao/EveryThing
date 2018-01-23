// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnSkin.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

#include "EveryThingAssetManager.h"

FGamePawnSkin::FGamePawnSkin(UStaticMeshComponent* StaticMeshComp)
{
	OwnerStaticMeshComp = StaticMeshComp;
}

void FGamePawnSkin::LoadGamePawnSkin()
{
	if (OwnerStaticMeshComp)
	{
		UEveryThingAssetManager* AssetManager = UEveryThingAssetManager::GetAssetManagerInstance();

		for (int32 i = 0; i < MaterialNames.Num(); ++i)
		{
			UMaterialInstanceConstant* MaterialInstance = AssetManager->GetMaterialFromName(MaterialNames[i]);
			if (MaterialInstance)
			{
				OwnerStaticMeshComp->SetMaterial(i, MaterialInstance);
			}
		}
	}
}

void FGamePawnSkin::UnloadGamePawnSkin()
{

}