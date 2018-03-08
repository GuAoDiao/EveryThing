// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnManager.h"

#include "Characters/GamePawn.h"
#include "EveryThingAssetManager.h"

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