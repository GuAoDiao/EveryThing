// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballPawn.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

#include "EveryThingAssetManager.h"

#include "Characters/GamePawns/Football/FootballForm.h"
#include "Characters/GamePawns/Football/FootballTestForm.h"

AFootballPawn::AFootballPawn()
{
	Quality = 500.f;
	QualityScale = 1.f;
	LinearDamping = 0.01f;
	AngularDamping = 5.f;

	ResetQualityAndDamping();
	
	UEveryThingAssetManager* AssetManager = UEveryThingAssetManager::GetAssetManagerInstance();

	UStaticMesh* FootballMesh = AssetManager->GetMeshFromName(TEXT("Football"));
	if (FootballMesh) { StaticMeshComp->SetStaticMesh(FootballMesh); }

	UMaterialInstanceConstant* FootballMaterialBlack = AssetManager->GetMaterialFromName(TEXT("FootballBlack_Fire"));
	UMaterialInstanceConstant* FootballMaterialWhite = AssetManager->GetMaterialFromName(TEXT("FootballWhite_Fire"));

	if (FootballMaterialBlack && FootballMaterialWhite)
	{
		StaticMeshComp->SetMaterial(0, FootballMaterialBlack);
		StaticMeshComp->SetMaterial(1, FootballMaterialWhite);
	}

	// base form
	AddGamePawnForm(new FFootballForm(this));
	// test form
	AddGamePawnForm(new FFootballTestForm(this));

	ServerToggleToNewPawnFormWithIndex(0);
}
