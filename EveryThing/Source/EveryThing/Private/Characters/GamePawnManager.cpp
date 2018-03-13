// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnManager.h"

#include "Characters/GamePawn.h"
#include "EveryThingAssetManager.h"
#include "Characters/Skin/GamePawnSkin.h"
#include "Characters/Form/GamePawnForm.h"


TMap<FName, FGamePawnSkinClassInfo*> UGamePawnManager::AllGamePawnSkinClassInfo;
TMap<FName, FGamePawnFormClassInfo*> UGamePawnManager::AllGamePawnFormClassInfo;

UGamePawnManager::UGamePawnManager()
{
	LoadRolesClassFromDatetable();
}

/// Game Pawn

void UGamePawnManager::LoadRolesClassFromDatetable()
{
	UDataTable* RoleNameDatatable = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("RolesInfo"));
	if (RoleNameDatatable)
	{
		TArray<FRoleInfo*> RolesNameDataInDatatable;
		RoleNameDatatable->GetAllRows<FRoleInfo>(TEXT("found all Roles Name in DataTable"), RolesNameDataInDatatable);
		for (FRoleInfo* RoleNameData : RolesNameDataInDatatable)
		{
			AllRolesInfo.Add(RoleNameData->Name, *RoleNameData);
		}
	}
}

TSoftClassPtr<AGamePawn> UGamePawnManager::GetRoleClassFromName(const FName& RolesName) const
{
	if (AllRolesInfo.Contains(RolesName))
	{
		return AllRolesInfo[RolesName].RoleClass;
	}

	return nullptr;
}

FName UGamePawnManager::GetRoleNameFromClass(UClass* RoleClass) const
{
	if (RoleClass)
	{
		for (TMap<FName, FRoleInfo>::TConstIterator It(AllRolesInfo); It; ++It)
		{
			UClass* CurrentClass = It.Value().RoleClass.Get();
			if (CurrentClass->IsChildOf(RoleClass) || RoleClass->IsChildOf(CurrentClass))
			{
				return It.Value().Name;
			}
		}
	}

	return NAME_None;
}

const TMap<FName, FRoleInfo>& UGamePawnManager::GetAllRolesInfo() const
{
	return AllRolesInfo;
}


FGamePawnSkin* UGamePawnManager::GetGamePawnSkinFromName(const FName& Name, UStaticMeshComponent* InStaticMeshComp)
{
	if (AllGamePawnSkinClassInfo.Contains(Name))
	{
		return AllGamePawnSkinClassInfo[Name]->GetClass(InStaticMeshComp);
	}
	return nullptr;
}

TArray<FName> UGamePawnManager::GetAllGamePawnSkinWithRoleName(const FName& RoleName)
{
	TArray<FName> Result;
	for (TMap<FName, FGamePawnSkinClassInfo*>::TConstIterator GamePawnSkinClassInfoItr(AllGamePawnSkinClassInfo); GamePawnSkinClassInfoItr; ++GamePawnSkinClassInfoItr)
	{
		if (GamePawnSkinClassInfoItr.Value() && GamePawnSkinClassInfoItr.Value()->RoleName == RoleName)
		{
			Result.AddUnique(GamePawnSkinClassInfoItr.Value()->SkinName);
		}
	}
	return Result;
}

void UGamePawnManager::RegisterGamePawnSkinWithName(const FName& Name, FGamePawnSkinClassInfo* ClassInfo)
{
	AllGamePawnSkinClassInfo.Add(Name, ClassInfo);
}

FGamePawnForm* UGamePawnManager::GetGamePawnFormFromName(const FName& Name, AGamePawn* InGamePawn)
{
	if (AllGamePawnFormClassInfo.Contains(Name))
	{
		return AllGamePawnFormClassInfo[Name]->GetClass(InGamePawn);
	}
	return nullptr;
}


TArray<FName> UGamePawnManager::GetAllGamePawnFormWithRoleName(const FName& RoleName)
{
	TArray<FName> Result;
	for (TMap<FName, FGamePawnFormClassInfo*>::TConstIterator GamePawnFormClassInfoItr(AllGamePawnFormClassInfo); GamePawnFormClassInfoItr; ++GamePawnFormClassInfoItr)
	{
		if (GamePawnFormClassInfoItr.Value() && GamePawnFormClassInfoItr.Value()->RoleName == RoleName)
		{
			Result.AddUnique(GamePawnFormClassInfoItr.Value()->FormName);
		}
	}
	return Result;
}


void UGamePawnManager::RegisterGamePawnFormWithName(const FName& Name, FGamePawnFormClassInfo* ClassInfo)
{
	AllGamePawnFormClassInfo.Add(Name, ClassInfo);
}