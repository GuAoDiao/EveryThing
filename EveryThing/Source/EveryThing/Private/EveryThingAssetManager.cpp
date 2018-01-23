// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingAssetManager.h"

#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystem.h"
#include "Materials/MaterialInstanceConstant.h"

UEveryThingAssetManager* UEveryThingAssetManager::AssetManager = nullptr;

UEveryThingAssetManager::UEveryThingAssetManager()
{
	UDataTable* AllDataTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/EveryThing/DataTable/DT_DataTable.DT_DataTable'"));
	if (AllDataTable)
	{
		TArray<FDataTableData*> AllDataTableInfo;
		AllDataTable->GetAllRows<FDataTableData>(TEXT("found all DataTable"), AllDataTableInfo);
		for (FDataTableData* DataTable : AllDataTableInfo)
		{
			AllDataTableAsset.Add(FName(*DataTable->Name), DataTable->DataTableClass);
		}
	}

	UDataTable* StaticMeshDatatable = GetDataTableFromName(TEXT("StaticMesh"));
	if (StaticMeshDatatable)
	{
		TArray<FStaticMeshData*> StaticMeshDataInDatatable;
		StaticMeshDatatable->GetAllRows<FStaticMeshData>(TEXT("found all StaticMesh DataTable"), StaticMeshDataInDatatable);
		for (FStaticMeshData* StaticMeshData : StaticMeshDataInDatatable)
		{
			AllMeshAsset.Add(FName(*StaticMeshData->Name), StaticMeshData->MeshClass);
		}
	}

	UDataTable* MaterialInstaneDatatable = GetDataTableFromName(TEXT("MaterialInstance"));
	if (MaterialInstaneDatatable)
	{
		TArray<FMaterialInstanceData*> MaterialInstanceDataInDatatable;
		MaterialInstaneDatatable->GetAllRows<FMaterialInstanceData>(TEXT("found all MaterialInstane DataTable"), MaterialInstanceDataInDatatable);
		for (FMaterialInstanceData* MaterialInstaneData : MaterialInstanceDataInDatatable)
		{
			AllMaterialInstanceAsset.Add(FName(*MaterialInstaneData->Name), MaterialInstaneData->MaterialInstanceClass);
		}
	}

	UDataTable* ParticleSystemDatatable = GetDataTableFromName(TEXT("ParticleSystem"));
	if (ParticleSystemDatatable)
	{
		TArray<FParticleSystemData*> ParticleSystemDataInDatatable;
		ParticleSystemDatatable->GetAllRows<FParticleSystemData>(TEXT("found all ParticleSystem DataTable"), ParticleSystemDataInDatatable);
		for (FParticleSystemData* ParticleSystemData : ParticleSystemDataInDatatable)
		{
			AllParticleAsset.Add(FName(*ParticleSystemData->Name), ParticleSystemData->ParticleSystemClass);
		}
	}
}
UEveryThingAssetManager::~UEveryThingAssetManager()
{
	AssetManager = nullptr;
	UE_LOG(LogTemp, Log, TEXT("-_- this is destroy asset manager"));
}

UEveryThingAssetManager* UEveryThingAssetManager::GetAssetManagerInstance()
{
	if (!AssetManager)
	{
		AssetManager = NewObject<UEveryThingAssetManager>((UObject*)GetTransientPackage(), TEXT("Blueprint'/Game/EveryThing/Blueprints/BP_AssetManager.BP_AssetManager'"));
		UE_LOG(LogTemp, Log, TEXT("-_- init EveryThing Asset Manager"));
	}

	check(AssetManager);

	return AssetManager;
}

void UEveryThingAssetManager::DestroyAssetManagerInstance()
{
	if (AssetManager) { AssetManager->BeginDestroy(); }
	AssetManager = nullptr;
}

UStaticMesh* UEveryThingAssetManager::GetMeshFromName(const FString& MeshName)
{
	FName MeskKey(*MeshName);
	if (!AllMeshAsset.Contains(MeskKey))
	{
		AllMeshAsset.Add(MeskKey, LoadObject<UStaticMesh>(nullptr, *FString::Printf(TEXT("StaticMesh'/Game/EveryThing/Meshes/SM_%s.SM_%s'"), *MeshName, *MeshName)));
	}
	return AllMeshAsset[MeskKey];
}


UMaterialInstanceConstant* UEveryThingAssetManager::GetMaterialFromName(const FString& MaterialName)
{
	FName MaterialKey(*MaterialName);
	if (!AllMaterialInstanceAsset.Contains(MaterialKey))
	{
		AllMaterialInstanceAsset.Add(MaterialKey, LoadObject<UMaterialInstanceConstant>(nullptr, *FString::Printf(TEXT("MaterialInstanceConstant'/Game/EveryThing/Materials/MI_%s.MI_%s'"), *MaterialName, *MaterialName)));
	}
	return AllMaterialInstanceAsset[MaterialKey];
}

UParticleSystem* UEveryThingAssetManager::GetParticleFromName(const FString& ParticleName)
{
	FName ParticleKey(*ParticleName);
	if (!AllParticleAsset.Contains(ParticleKey))
	{
		AllParticleAsset.Add(ParticleKey, LoadObject<UParticleSystem>(nullptr, *FString::Printf(TEXT("ParticleSystem'/Game/EveryThing/Particles/P_%s.P_%s'"), *ParticleName, *ParticleName)));
	}
	return AllParticleAsset[ParticleKey];
}

UDataTable* UEveryThingAssetManager::GetDataTableFromName(const FString& DataTableName)
{
	FName DataTableKey(*DataTableName);
	if (!AllDataTableAsset.Contains(DataTableKey))
	{
		AllDataTableAsset.Add(DataTableKey, LoadObject<UDataTable>(nullptr, *FString::Printf(TEXT("DataTable'/Game/EveryThing/DataTable/DT_%s.DT_%s'"), *DataTableName, *DataTableName)));
	}
	return AllDataTableAsset[DataTableKey];
}
