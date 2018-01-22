// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EveryThingAssetManager.generated.h"

class UStaticMesh;
class UParticleSystem;
class UMaterialInstanceConstant;

USTRUCT(BlueprintType)
struct FStaticMeshData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* MeshClass;
};

USTRUCT(BlueprintType)
struct FMaterialInstanceData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInstanceConstant* MaterialInstanceClass;
};

USTRUCT(BlueprintType)
struct FParticleSystemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UParticleSystem* ParticleSystemClass;
};

USTRUCT(BlueprintType)
struct FDataTableData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDataTable* DataTableClass;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class EVERYTHING_API UEveryThingAssetManager : public UObject
{
	GENERATED_BODY()
	
protected:
	UEveryThingAssetManager();
	~UEveryThingAssetManager();
public:
	static UEveryThingAssetManager* GetAssetManagerInstance();
	static void DestroyAssetManagerInstance();
private:
	static UEveryThingAssetManager* AssetManager;

public:
	UStaticMesh* GetMeshFromName(const FString& MeshName);
	UMaterialInstanceConstant* GetMaterialFromName(const FString& MaterialName);
	UParticleSystem* GetParticleFromName(const FString& ParticleName);
	UDataTable* GetDataTableFromName(const FString& DataTableName);

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FName, UStaticMesh*> AllMeshAsset;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FName, UParticleSystem*> AllParticleAsset;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FName, UDataTable*> AllDataTableAsset;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FName, UMaterialInstanceConstant*> AllMaterialInstanceAsset;
};
