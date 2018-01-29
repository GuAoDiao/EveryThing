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

	LoadMeshFromDatatable();

	LoadMaterialFromDatatable();

	LoadParticleFromDatatable();

	LoadUserWidgetFromDataable();
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
			AllMeshAsset.Add(FName(*StaticMeshData->Name), StaticMeshData->MeshClass);
		}
	}
}

void UEveryThingAssetManager::NeededMeshFromName(const FString& MeshName)
{
	GetMeshFromName(MeshName, false);
}

UStaticMesh* UEveryThingAssetManager::GetMeshFromName(const FString& MeshName, bool bIsNeedForce)
{
	FName MeskKey(*MeshName);
	if (!AllMeshAsset.Contains(MeskKey))
	{
		AllMeshAsset.Add(MeskKey, TSoftObjectPtr<UStaticMesh>(FString::Printf(TEXT("StaticMesh'/Game/EveryThing/Meshes/SM_%s.SM_%s'"), *MeshName, *MeshName)));
	}

	if (bIsNeedForce)
	{
		return AllMeshAsset[MeskKey].LoadSynchronous();
	}
	else
	{
		OwnerStreamableManager.RequestAsyncLoad(AllMeshAsset[MeskKey].ToSoftObjectPath());

		return AllMeshAsset[MeskKey].Get();
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
			AllMaterialInstanceAsset.Add(FName(*MaterialInstaneData->Name), MaterialInstaneData->MaterialInstanceClass);
		}
	}
}

void UEveryThingAssetManager::NeededMaterialFromName(const FString& MaterialName)
{
	GetMaterialFromName(MaterialName, false);
}

UMaterialInstanceConstant* UEveryThingAssetManager::GetMaterialFromName(const FString& MaterialName, bool bIsNeedForce)
{
	FName MaterialKey(*MaterialName);
	if (!AllMaterialInstanceAsset.Contains(MaterialKey))
	{
		AllMaterialInstanceAsset.Add(MaterialKey, TSoftObjectPtr<UMaterialInstanceConstant>(FString::Printf(TEXT("MaterialInstanceConstant'/Game/EveryThing/Materials/MI_%s.MI_%s'"), *MaterialName, *MaterialName)));
	}

	if (bIsNeedForce)
	{
		return AllMaterialInstanceAsset[MaterialKey].LoadSynchronous();
	}
	else
	{
		OwnerStreamableManager.RequestAsyncLoad(AllMaterialInstanceAsset[MaterialKey].ToSoftObjectPath());

		return AllMaterialInstanceAsset[MaterialKey].Get();
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
			AllParticleAsset.Add(FName(*ParticleSystemData->Name), ParticleSystemData->ParticleSystemClass);
		}
	}
}

UParticleSystem* UEveryThingAssetManager::GetParticleFromName(const FString& ParticleName, bool bIsNeedForce)
{
	FName ParticleKey(*ParticleName);
	if (!AllParticleAsset.Contains(ParticleKey))
	{
		AllParticleAsset.Add(ParticleKey, TSoftObjectPtr<UParticleSystem>(FString::Printf(TEXT("ParticleSystem'/Game/EveryThing/Particles/P_%s.P_%s'"), *ParticleName, *ParticleName)));
	}

	if (bIsNeedForce)
	{
		return AllParticleAsset[ParticleKey].LoadSynchronous();
	}
	else
	{
		OwnerStreamableManager.RequestAsyncLoad(AllParticleAsset[ParticleKey].ToSoftObjectPath());

		return AllParticleAsset[ParticleKey].Get();
	}
}

void UEveryThingAssetManager::NeededParticleFromName(const FString& ParticleName)
{
	GetParticleFromName(ParticleName, false);
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
			AllUserWidgetAsset.Add(FName(*UserWidgetData->Name), UserWidgetData->UserWidgetClass);
		}
	}
}


TSubclassOf<UUserWidget> UEveryThingAssetManager::GetUserWidgetFromName(const FString& UserWidgetName)
{
	FName UserWidgetKey(*UserWidgetName);
	if (!AllUserWidgetAsset.Contains(UserWidgetKey))
	{
		AllUserWidgetAsset.Add(UserWidgetKey, TSoftClassPtr<UUserWidget>(FString::Printf(TEXT("WidgetBlueprint'/Game/EveryThing/UI/W_%s.W_%s_C'"), *UserWidgetName, *UserWidgetName)));
	}
	return AllUserWidgetAsset[UserWidgetKey].LoadSynchronous();
}

//////////////////////////////////////////////////////////////////////////
/// Data table
UDataTable* UEveryThingAssetManager::GetDataTableFromName(const FString& DataTableName)
{
	FName DataTableKey(*DataTableName);
	if (!AllDataTableAsset.Contains(DataTableKey))
	{
		AllDataTableAsset.Add(DataTableKey, TSoftObjectPtr<UDataTable>(FString::Printf(TEXT("DataTable'/Game/EveryThing/DataTable/DT_%s.DT_%s'"), *DataTableName, *DataTableName)));
	}
	return AllDataTableAsset[DataTableKey].LoadSynchronous();
}
