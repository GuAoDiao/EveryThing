// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingAssetManager.h"

#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystem.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Characters/GamePawnManager.h"

UEveryThingAssetManager* UEveryThingAssetManager::AssetManager = nullptr;

UEveryThingAssetManager::UEveryThingAssetManager()
{
	SetFlags(RF_Standalone);
	AssetManager = this;

	UDataTable* AllDataTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/EveryThing/DataTable/DT_DataTable.DT_DataTable'"));
	if (AllDataTable)
	{
		TArray<FDataTableData*> AllDataTableInfo;
		AllDataTable->GetAllRows<FDataTableData>(TEXT("found all DataTable"), AllDataTableInfo);
		for (FDataTableData* DataTable : AllDataTableInfo)
		{
			AllDataTableAsset.Add(DataTable->Name, DataTable->DataTableClass);
		}
	}

	LoadMeshFromDatatable();

	LoadMaterialFromDatatable();

	LoadParticleFromDatatable();

	LoadUserWidgetFromDataable();
}

UEveryThingAssetManager::~UEveryThingAssetManager()
{
	AssetManager = nullptr;
	if (GamePawnManager) { GamePawnManager->BeginDestroy(); }
}

UEveryThingAssetManager* UEveryThingAssetManager::GetAssetManagerInstance()
{
	if (!AssetManager)
	{
		AssetManager = NewObject<UEveryThingAssetManager>((UObject*)GetTransientPackage(), TEXT("Blueprint'/Game/EveryThing/Blueprints/BP_AssetManager.BP_AssetManager'"));
	}

	checkf(AssetManager, TEXT("The Asset Manager must be exists."));

	return AssetManager;
}

void UEveryThingAssetManager::DestroyAssetManagerInstance()
{
	if (AssetManager) { AssetManager->BeginDestroy(); }
	AssetManager = nullptr;
}

//////////////////////////////////////////////////////////////////////////
/// Mesh
void UEveryThingAssetManager::LoadMeshFromDatatable()
{
	UDataTable* StaticMeshDatatable = GetDataTableFromName(TEXT("StaticMesh"));
	if (StaticMeshDatatable)
	{
		TArray<FStaticMeshData*> StaticMeshDataInDatatable;
		StaticMeshDatatable->GetAllRows<FStaticMeshData>(TEXT("found all StaticMesh DataTable"), StaticMeshDataInDatatable);
		for (FStaticMeshData* StaticMeshData : StaticMeshDataInDatatable)
		{
			AllMeshAsset.Add(StaticMeshData->Name, StaticMeshData->MeshClass);
		}
	}
}

void UEveryThingAssetManager::NeededMeshFromName(const FName& MeshName) { GetMeshFromName(MeshName, false); }
UStaticMesh* UEveryThingAssetManager::GetMeshFromName(const FName& MeshName, bool bIsNeedForce)
{
	if (!AllMeshAsset.Contains(MeshName))
	{
		AllMeshAsset.Add(MeshName, TSoftObjectPtr<UStaticMesh>(FString::Printf(TEXT("StaticMesh'/Game/EveryThing/Meshes/SM_%s.SM_%s'"), *MeshName.ToString(), *MeshName.ToString())));
	}

	if (bIsNeedForce)
	{
		return AllMeshAsset[MeshName].LoadSynchronous();
	}
	else
	{
		UStaticMesh* Result = AllMeshAsset[MeshName].Get();
		if (!Result) { OwnerStreamableManager.RequestAsyncLoad(AllMeshAsset[MeshName].ToSoftObjectPath()); }
		return Result;
	}
}


//////////////////////////////////////////////////////////////////////////
/// Material instance

void UEveryThingAssetManager::LoadMaterialFromDatatable()
{
	UDataTable* MaterialInstaneDatatable = GetDataTableFromName(TEXT("MaterialInstance"));
	if (MaterialInstaneDatatable)
	{
		TArray<FMaterialInstanceData*> MaterialInstanceDataInDatatable;
		MaterialInstaneDatatable->GetAllRows<FMaterialInstanceData>(TEXT("found all MaterialInstane DataTable"), MaterialInstanceDataInDatatable);
		for (FMaterialInstanceData* MaterialInstaneData : MaterialInstanceDataInDatatable)
		{
			AllMaterialInstanceAsset.Add(MaterialInstaneData->Name, MaterialInstaneData->MaterialInstanceClass);
		}
	}
}

void UEveryThingAssetManager::NeededMaterialFromName(const FName& MaterialName) { GetMaterialFromName(MaterialName, false); }
UMaterialInstanceConstant* UEveryThingAssetManager::GetMaterialFromName(const FName& MaterialName, bool bIsNeedForce)
{
	if (!AllMaterialInstanceAsset.Contains(MaterialName))
	{
		AllMaterialInstanceAsset.Add(MaterialName, TSoftObjectPtr<UMaterialInstanceConstant>(FString::Printf(TEXT("MaterialInstanceConstant'/Game/EveryThing/Materials/MI_%s.MI_%s'"), *MaterialName.ToString(), *MaterialName.ToString())));
	}

	if (bIsNeedForce)
	{
		return AllMaterialInstanceAsset[MaterialName].LoadSynchronous();
	}
	else
	{
		UMaterialInstanceConstant* Result = AllMaterialInstanceAsset[MaterialName].Get();
		if (!Result) { OwnerStreamableManager.RequestAsyncLoad(AllMaterialInstanceAsset[MaterialName].ToSoftObjectPath()); }
		return Result;
	}
}



//////////////////////////////////////////////////////////////////////////
/// Particle

void UEveryThingAssetManager::LoadParticleFromDatatable()
{
	UDataTable* ParticleSystemDatatable = GetDataTableFromName(TEXT("ParticleSystem"));
	if (ParticleSystemDatatable)
	{
		TArray<FParticleSystemData*> ParticleSystemDataInDatatable;
		ParticleSystemDatatable->GetAllRows<FParticleSystemData>(TEXT("found all ParticleSystem DataTable"), ParticleSystemDataInDatatable);
		for (FParticleSystemData* ParticleSystemData : ParticleSystemDataInDatatable)
		{
			AllParticleAsset.Add(ParticleSystemData->Name, ParticleSystemData->ParticleSystemClass);
		}
	}
}

void UEveryThingAssetManager::NeededParticleFromName(const FName& ParticleName) { GetParticleFromName(ParticleName, false); }
UParticleSystem* UEveryThingAssetManager::GetParticleFromName(const FName& ParticleName, bool bIsNeedForce)
{
	if (!AllParticleAsset.Contains(ParticleName))
	{
		AllParticleAsset.Add(ParticleName, TSoftObjectPtr<UParticleSystem>(FString::Printf(TEXT("ParticleSystem'/Game/EveryThing/Particles/P_%s.P_%s'"), *ParticleName.ToString(), *ParticleName.ToString())));
	}

	if (bIsNeedForce)
	{
		return AllParticleAsset[ParticleName].LoadSynchronous();
	}
	else
	{
		UParticleSystem* Result = AllParticleAsset[ParticleName].Get();
		if (!Result) { OwnerStreamableManager.RequestAsyncLoad(AllParticleAsset[ParticleName].ToSoftObjectPath()); }
		return Result;
	}
}



//////////////////////////////////////////////////////////////////////////
/// User Widget

void UEveryThingAssetManager::LoadUserWidgetFromDataable()
{
	UDataTable* UserWidgetDatatable = GetDataTableFromName(TEXT("UserWidget"));
	if (UserWidgetDatatable)
	{
		TArray<FUserWidgetData*> UserWidgetDataInDatatable;
		UserWidgetDatatable->GetAllRows<FUserWidgetData>(TEXT("found all UserWidget DataTable"), UserWidgetDataInDatatable);
		for (FUserWidgetData* UserWidgetData : UserWidgetDataInDatatable)
		{
			AllUserWidgetAsset.Add(UserWidgetData->Name, UserWidgetData->UserWidgetClass);
		}
	}
}


TSubclassOf<UUserWidget> UEveryThingAssetManager::GetUserWidgetFromName(const FName& UserWidgetName)
{
	if (!AllUserWidgetAsset.Contains(UserWidgetName))
	{
		AllUserWidgetAsset.Add(UserWidgetName, TSoftClassPtr<UUserWidget>(FString::Printf(TEXT("WidgetBlueprint'/Game/EveryThing/UI/W_%s.W_%s_C'"), *UserWidgetName.ToString(), *UserWidgetName.ToString())));
	}
	return AllUserWidgetAsset[UserWidgetName].LoadSynchronous();
}

//////////////////////////////////////////////////////////////////////////
/// Data table
UDataTable* UEveryThingAssetManager::GetDataTableFromName(const FName& DataTableName)
{
	if (!AllDataTableAsset.Contains(DataTableName))
	{
		AllDataTableAsset.Add(DataTableName, TSoftObjectPtr<UDataTable>(FString::Printf(TEXT("DataTable'/Game/EveryThing/DataTable/DT_%s.DT_%s'"), *DataTableName.ToString(), *DataTableName.ToString())));
	}
	return AllDataTableAsset[DataTableName].LoadSynchronous();
}

//////////////////////////////////////////////////////////////////////////
/// Game Pawn
UGamePawnManager* UEveryThingAssetManager::GetGamePawnManager()
{
	if (!GamePawnManager)
	{
		GamePawnManager = NewObject<UGamePawnManager>();
	}

	checkf(GamePawnManager, TEXT("The GamePawnManager must exists"));

	return GamePawnManager;
}