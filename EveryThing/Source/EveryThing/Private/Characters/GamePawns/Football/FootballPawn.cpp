// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballPawn.h"

#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"

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
	
	UStaticMesh* FootballMesh = UEveryThingAssetManager::GetAssetManagerInstance()->GetMeshFromName(TEXT("Football"));
	if (FootballMesh) { StaticMeshComp->SetStaticMesh(FootballMesh); }

	// base form
	AddGamePawnForm(new FFootballForm(this));
	// test form
	AddGamePawnForm(new FFootballTestForm(this));

	ServerToggleToNewPawnFormWithIndex(0);
}
