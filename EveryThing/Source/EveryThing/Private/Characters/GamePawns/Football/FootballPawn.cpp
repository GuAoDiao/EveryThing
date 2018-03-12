// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballPawn.h"

#include "Components/StaticMeshComponent.h"

#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"

AFootballPawn::AFootballPawn()
{
	ResetInfoFromDataTable("Football");
		
	UStaticMesh* FootballMesh = UEveryThingAssetManager::GetAssetManagerInstance()->GetMeshFromName(TEXT("Football"));
	if (FootballMesh) { StaticMeshComp->SetStaticMesh(FootballMesh); }

	FGamePawnSkin* GamePawnSkin;
	GamePawnSkin = UGamePawnManager::GetGamePawnSkinFromName("FootballSkin", StaticMeshComp);
	if (GamePawnSkin) { AddGamePawnSkin(GamePawnSkin); }
	GamePawnSkin = UGamePawnManager::GetGamePawnSkinFromName("FootballFireSkin", StaticMeshComp);
	if (GamePawnSkin) { AddGamePawnSkin(GamePawnSkin); }

	ToggleToNewPawnSkin(0);

	FGamePawnForm* GamePawnForm;
	GamePawnForm = UGamePawnManager::GetGamePawnFormFromName("FootballForm", this);
	if (GamePawnForm) { AddGamePawnForm(GamePawnForm); }
	GamePawnForm = UGamePawnManager::GetGamePawnFormFromName("FootballTestForm", this);
	if (GamePawnForm) { AddGamePawnForm(GamePawnForm); }

	ToggleToNewPawnForm(0);
}
