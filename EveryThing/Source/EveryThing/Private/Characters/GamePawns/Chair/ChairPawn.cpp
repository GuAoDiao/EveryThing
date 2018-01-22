// Fill out your copyright notice in the Description page of Project Settings.

#include "ChairPawn.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

#include "EveryThingAssetManager.h"

AChairPawn::AChairPawn()
{

	UStaticMesh* ChairMesh = UEveryThingAssetManager::GetAssetManagerInstance()->GetMeshFromName(TEXT("Chair"));
	if (ChairMesh) { StaticMeshComp->SetStaticMesh(ChairMesh); }

	UEveryThingAssetManager* AssetManager = UEveryThingAssetManager::GetAssetManagerInstance();
	
	UMaterialInstanceConstant* ChairWoodMaterial = AssetManager->GetMaterialFromName("ChairWood");
	UMaterialInstanceConstant* ChairMatMaterial = AssetManager->GetMaterialFromName("ChairMat");
	UMaterialInstanceConstant* ChairTidyMaterial = AssetManager->GetMaterialFromName("ChairTidy");
	if (ChairWoodMaterial && ChairMatMaterial && ChairTidyMaterial)
	{
		StaticMeshComp->SetMaterial(0, ChairMatMaterial);
		StaticMeshComp->SetMaterial(1, ChairWoodMaterial);
		StaticMeshComp->SetMaterial(2, ChairTidyMaterial);
	}

	Quality = 1000.f;
	QualityScale = 1.f;

	LinearDamping = 0.01f;
	AngularDamping = 10.f;

	ResetQualityAndDamping();
}