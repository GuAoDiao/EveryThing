// Fill out your copyright notice in the Description page of Project Settings.

#include "ChairPawn.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"

AChairPawn::AChairPawn()
{
	RoleName = "Chair";

	ResetInfoFromDataTable("Chair");

	UEveryThingAssetManager* AssetManager = UEveryThingAssetManager::GetAssetManagerInstance();
	UGamePawnManager* GamePawnManager = AssetManager->GetGamePawnManager();

	UStaticMesh* ChairMesh = AssetManager->GetMeshFromName("Chair");
	if (ChairMesh) { StaticMeshComp->SetStaticMesh(ChairMesh); }

	AllGamePawnSkinName = GamePawnManager->GetAllGamePawnSkinWithRoleName(RoleName);

	AllHaveGamePawnSkinName.Add("ChairSkin");
	ToggleToTargetSkin("ChairSkin");
}