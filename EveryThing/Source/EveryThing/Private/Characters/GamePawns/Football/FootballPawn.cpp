// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballPawn.h"

#include "Components/StaticMeshComponent.h"

#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"

AFootballPawn::AFootballPawn()
{
	RoleName = "Football";

	ResetInfoFromDataTable(RoleName);
		
	UStaticMesh* FootballMesh = UEveryThingAssetManager::GetAssetManagerInstance()->GetMeshFromName(RoleName);
	if (FootballMesh) { StaticMeshComp->SetStaticMesh(FootballMesh); }

	AllGamePawnSkinName = UGamePawnManager::GetAllGamePawnSkinWithRoleName(RoleName);
	
	AllHaveGamePawnSkinName.Add("FootballSkin");
	ToggleToTargetSkin("FootballSkin");

	AllGamePawnFormName = UGamePawnManager::GetAllGamePawnFormWithRoleName(RoleName);

	AllHaveGamePawnFormName.Add("FootballForm");
	ToggleToTargetForm("FootballForm");
}
