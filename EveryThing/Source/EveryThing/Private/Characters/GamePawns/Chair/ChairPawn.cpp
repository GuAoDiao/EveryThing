// Fill out your copyright notice in the Description page of Project Settings.

#include "ChairPawn.h"

#include "Components/StaticMeshComponent.h"

#include "EveryThingAssetManager.h"

AChairPawn::AChairPawn()
{
	UStaticMesh* ChairMesh = UEveryThingAssetManager::GetAssetManagerInstance()->GetMeshFromName(TEXT("Chair"));
	if (ChairMesh) { StaticMeshComp->SetStaticMesh(ChairMesh); }

	Quality = 1000.f;
	QualityScale = 1.f;

	LinearDamping = 0.01f;
	AngularDamping = 10.f;

	ResetQualityAndDamping();
}