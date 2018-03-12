// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EveryThingTypes.h"
#include "GamePawnManager.generated.h"

class AGamePawn;
class FGamePawnSkin;
class FGamePawnForm;
class UStaticMeshComponent;
class FGamePawnSkinClassInfo;
class FGamePawnFormClassInfo;

typedef FGamePawnSkin*(*GamePawnSkinCreateFunc)(UStaticMeshComponent* /*InStatichMeshComp*/);
typedef FGamePawnForm*(*GamePawnFormCreateFunc)(AGamePawn* /*InGamePawn*/);

/**
 * 
 */
UCLASS()
class EVERYTHING_API UGamePawnManager : public UObject
{
	GENERATED_BODY()
	
public:
	UGamePawnManager();
	

	//////////////////////////////////////////////////////////////////////////
	/// Role Info
public:
	void LoadRolesClassFromDatetable();
	TSoftClassPtr<AGamePawn> GetRoleClassFromName(const FName& Name) const;
	FName GetRoleNameFromClass(UClass* RoleClass) const;
	const TMap<FName, FRoleInfo>& GetAllRolesInfo() const;
private:
	TMap<FName, FRoleInfo> AllRolesInfo;
	//////////////////////////////////////////////////////////////////////////
	/// Role Skin
public:

	static FGamePawnSkin* GetGamePawnSkinFromName(const FName& Name, UStaticMeshComponent* InStaticMeshComp);
	static void RegisterGamePawnSkinWithName(const FName& Name, FGamePawnSkinClassInfo* ClassInfo);

private:
	static TMap<FName, FGamePawnSkinClassInfo*> AllGamePawnSkinClassInfo;
	//////////////////////////////////////////////////////////////////////////
	/// Role Form
public:
	static FGamePawnForm* GetGamePawnFormFromName(const FName& Name, AGamePawn* InGamePawn);
	static void RegisterGamePawnFormWithName(const FName& Name, FGamePawnFormClassInfo* ClassInfo);

private:
	static TMap<FName, FGamePawnFormClassInfo*> AllGamePawnFormClassInfo;
};