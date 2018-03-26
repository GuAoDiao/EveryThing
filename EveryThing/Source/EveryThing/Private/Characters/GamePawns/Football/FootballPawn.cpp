// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballPawn.h"

#include "Components/StaticMeshComponent.h"

#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"

AFootballPawn::AFootballPawn()
{
	RoleName = "Football";

	ResetInfoFromDataTable(RoleName);
	
	UEveryThingAssetManager* AssetManager = UEveryThingAssetManager::GetAssetManagerInstance();
	UGamePawnManager* GamePawnManager = AssetManager->GetGamePawnManager();

	UStaticMesh* FootballMesh = AssetManager->GetMeshFromName(RoleName);
	if (FootballMesh) { StaticMeshComp->SetStaticMesh(FootballMesh); }

	AllGamePawnSkinName = GamePawnManager->GetAllGamePawnSkinWithRoleName(RoleName);
	AllGamePawnFormName = GamePawnManager->GetAllGamePawnFormWithRoleName(RoleName);
	
	ResetDefaultSkinAndFormFromDataTable();
}
