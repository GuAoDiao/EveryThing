// Fill out your copyright notice in the Description page of Project Settings.

#include "ChairPawn.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

#include "EveryThingAssetManager.h"

AChairPawn::AChairPawn()
{
	RoleName = "Chair";

	ResetInfoFromDataTable("Chair");

	UEveryThingAssetManager* AssetManager = UEveryThingAssetManager::GetAssetManagerInstance();

	UStaticMesh* ChairMesh = AssetManager->GetMeshFromName("Chair");
	if (ChairMesh) { StaticMeshComp->SetStaticMesh(ChairMesh); }

	
	UMaterialInstanceConstant* ChairWoodMaterial = AssetManager->GetMaterialFromName("ChairWood");
	UMaterialInstanceConstant* ChairMatMaterial = AssetManager->GetMaterialFromName("ChairMat");
	UMaterialInstanceConstant* ChairTidyMaterial = AssetManager->GetMaterialFromName("ChairTidy");

	if (ChairWoodMaterial && ChairMatMaterial && ChairTidyMaterial)
	{
		StaticMeshComp->SetMaterial(0, ChairMatMaterial);
		StaticMeshComp->SetMaterial(1, ChairWoodMaterial);
		StaticMeshComp->SetMaterial(2, ChairTidyMaterial);
	}
}