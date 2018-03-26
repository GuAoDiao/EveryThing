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
	void LoadAllRolesInfo();
	TSoftClassPtr<AGamePawn> GetRoleClassFromName(const FName& Name) const;
	FName GetRoleNameFromClass(UClass* RoleClass) const;
	bool GetRoleInfoFromName(const FName& SkinName, const FRoleInfo* & OutRoleInfo) const;
	const TMap<FName, FRoleInfo>& GetAllRolesInfo() const;
private:
	TMap<FName, FRoleInfo> AllRolesInfo;

	//////////////////////////////////////////////////////////////////////////
	/// Role Skin
public:
	void LoadAllRoleSkinInfo();
	TArray<FName> GetAllGamePawnSkinWithRoleName(const FName& RoleName);
	bool GetRoleSkinInfoFromName(const FName& SkinName, const FRoleSkinInfo* & OutRoleSkinInfo) const;
private:
	TMap<FName, FRoleFormInfo> AllRoleFormInfo;

	//////////////////////////////////////////////////////////////////////////
	/// Role Form
public:
	void LoadAllRoleFormInfo();
	TArray<FName> GetAllGamePawnFormWithRoleName(const FName& RoleName);
	bool GetRoleFormInfoFromName(const FName& SkinName, const FRoleFormInfo* & OutRoleFormInfo) const;
private:
	TMap<FName, FRoleSkinInfo> AllRoleSkinInfo;

	//////////////////////////////////////////////////////////////////////////
	/// Role Display Info
public:
	bool GetRoleDisplayInfoFromName(const FName& RoleName, FRoleDisplayInfo const*& OutRoleDisplayInfo);
private:
	class UDataTable* RoleDisplayInfoDT;
	TMap<FName, FRoleDisplayInfo> AllRoleDisplayInfo;


	//////////////////////////////////////////////////////////////////////////
	/// Skin and Form class info
public:
	static FGamePawnSkin* CreateGamePawnSkinFromName(const FName& Name, UStaticMeshComponent* InStaticMeshComp);
	static void RegisterGamePawnSkinWithName(const FName& Name, FGamePawnSkinClassInfo* ClassInfo);

	static FGamePawnForm* CreateGamePawnFormFromName(const FName& Name, AGamePawn* InGamePawn);
	static void RegisterGamePawnFormWithName(const FName& Name, FGamePawnFormClassInfo* ClassInfo);

private:
	static TMap<FName, FGamePawnSkinClassInfo*> AllGamePawnSkinClassInfo;
	static TMap<FName, FGamePawnFormClassInfo*> AllGamePawnFormClassInfo;
};