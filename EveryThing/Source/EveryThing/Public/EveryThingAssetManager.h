// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"

#include "Blueprint/UserWidget.h"

#include "EveryThingAssetManager.generated.h"

class UStaticMesh;
class UParticleSystem;
class UMaterialInstanceConstant;
class UUserWidget;

USTRUCT(BlueprintType)
struct FStaticMeshData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UStaticMesh> MeshClass;
};

USTRUCT(BlueprintType)
struct FMaterialInstanceData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UMaterialInstanceConstant> MaterialInstanceClass;
};

USTRUCT(BlueprintType)
struct FParticleSystemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UParticleSystem> ParticleSystemClass;
};

USTRUCT(BlueprintType)
struct FUserWidgetData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftClassPtr<UUserWidget> UserWidgetClass;
};

USTRUCT(BlueprintType)
struct FDataTableData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UDataTable> DataTableClass;
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
	
	FStreamableManager OwnerStreamableManager;
	//////////////////////////////////////////////////////////////////////////
	/// Mesh
public:
	void NeededMeshFromName(const FString& MeshName);
	UStaticMesh* GetMeshFromName(const FString& MeshName, bool bIsNeedForce = true);
private:
	void LoadMeshFromDatatable();
	TMap<FName, TSoftObjectPtr<UStaticMesh>> AllMeshAsset;


	//////////////////////////////////////////////////////////////////////////
	/// Material instance
public:
	void NeededMaterialFromName(const FString& MaterialName);
	UMaterialInstanceConstant* GetMaterialFromName(const FString& MaterialName, bool bIsNeedForce = false);

private:
	void LoadMaterialFromDatatable();
	TMap<FName, TSoftObjectPtr<UMaterialInstanceConstant>> AllMaterialInstanceAsset;

	//////////////////////////////////////////////////////////////////////////
	/// Particle
public:
	void NeededParticleFromName(const FString& ParticleName);
	UParticleSystem* GetParticleFromName(const FString& ParticleName, bool bIsNeedForce = false);

private:
	void LoadParticleFromDatatable();
	TMap<FName, TSoftObjectPtr<UParticleSystem>> AllParticleAsset;

	//////////////////////////////////////////////////////////////////////////
	/// User Widget
public:

	void LoadUserWidgetFromDataable();
	TSubclassOf<UUserWidget> GetUserWidgetFromName(const FString& UserWidgetName);

private:	
	TMap<FName, TSoftClassPtr<UUserWidget>> AllUserWidgetAsset;

	//////////////////////////////////////////////////////////////////////////
	/// Data table

public:
	UDataTable* GetDataTableFromName(const FString& DataTableName);

private:
	TMap<FName, TSoftObjectPtr<UDataTable>> AllDataTableAsset;
};
