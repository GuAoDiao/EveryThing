// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballPawn.h"

#include "Components/StaticMeshComponent.h"

#include "EveryThingAssetManager.h"

#include "Characters/GamePawns/Football/FootballForm.h"
#include "Characters/GamePawns/Football/FootballTestForm.h"

#include "Characters/GamePawns/Football/FootballSkin.h"
#include "Characters/GamePawns/Football/FootballFireSkin.h"

AFootballPawn::AFootballPawn()
{
	ResetInfoFromDataTable("Football");
		
	UStaticMesh* FootballMesh = UEveryThingAssetManager::GetAssetManagerInstance()->GetMeshFromName(TEXT("Football"));
	if (FootballMesh) { StaticMeshComp->SetStaticMesh(FootballMesh); }

	AddGamePawnSkin(new FFootballSkin(StaticMeshComp));
	AddGamePawnSkin(new FFootballFireSkin(StaticMeshComp));

	ToggleToNewPawnSkin(0);

	// base form
	AddGamePawnForm(new FFootballForm(this));
	// test form
	AddGamePawnForm(new FFootballTestForm(this));

	ToggleToNewPawnForm(0);
}
