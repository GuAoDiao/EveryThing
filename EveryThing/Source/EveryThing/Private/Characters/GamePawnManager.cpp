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
	LoadAllRolesInfo();
	LoadAllRoleSkinInfo();
	LoadAllRoleFormInfo();

	RoleDisplayInfoDT = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("RoleDisplayInfo"));
}

//////////////////////////////////////////////////////////////////////////
/// Load
void UGamePawnManager::LoadAllRolesInfo()
{
	UDataTable* RolesInfoDT = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("RolesInfo"));
	if (RolesInfoDT)
	{
		TArray<FRoleInfo*> RolesInfoInDatatable;
		RolesInfoDT->GetAllRows<FRoleInfo>(TEXT("found all Roles Name in DataTable"), RolesInfoInDatatable);
		for (FRoleInfo* RoleInfo : RolesInfoInDatatable)
		{
			AllRolesInfo.Add(RoleInfo->Name, *RoleInfo);
		}
	}
}

void UGamePawnManager::LoadAllRoleSkinInfo()
{
	UDataTable* RoleSkinInfoDT = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("RoleSkinInfo"));
	if (RoleSkinInfoDT)
	{
		TArray<FRoleSkinInfo*> RoleSkinInfoInDatatable;
		RoleSkinInfoDT->GetAllRows<FRoleSkinInfo>(TEXT("found all Roles Name in DataTable"), RoleSkinInfoInDatatable);
		for (FRoleSkinInfo* SkinInfo : RoleSkinInfoInDatatable)
		{
			AllRoleSkinInfo.Add(SkinInfo->Name, *SkinInfo);
		}
	}
}

void UGamePawnManager::LoadAllRoleFormInfo()
{

	UDataTable* RoleFormInfoDT = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("RoleFormInfo"));
	if (RoleFormInfoDT)
	{
		TArray<FRoleFormInfo*> RoleFormInfoInDatatable;
		RoleFormInfoDT->GetAllRows<FRoleFormInfo>(TEXT("found all Roles Name in DataTable"), RoleFormInfoInDatatable);
		for (FRoleFormInfo* RoleFormInfo : RoleFormInfoInDatatable)
		{
			AllRoleFormInfo.Add(RoleFormInfo->Name, *RoleFormInfo);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/// Role Info
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

bool UGamePawnManager::GetRoleInfoFromName(const FName& RoleName, const FRoleInfo* & OutRoleInfo) const
{
	if (AllRolesInfo.Contains(RoleName))
	{
		OutRoleInfo = &AllRolesInfo[RoleName];
		return true;
	}

	return false;
}

const TMap<FName, FRoleInfo>& UGamePawnManager::GetAllRolesInfo() const
{
	return AllRolesInfo;
}

//////////////////////////////////////////////////////////////////////////
/// Role Skin Info

TArray<FName> UGamePawnManager::GetAllGamePawnSkinWithRoleName(const FName& RoleName)
{
	TArray<FName> Result;
	for (TMap<FName, FRoleSkinInfo>::TConstIterator It(AllRoleSkinInfo); It; ++It)
	{
		if (It.Value().RoleName == RoleName)
		{
			Result.AddUnique(It.Key());
		}
	}
	return Result;
}

bool UGamePawnManager::GetRoleSkinInfoFromName(const FName& SkinName, const FRoleSkinInfo* & OutRoleSkinInfo) const
{
	if (AllRoleSkinInfo.Contains(SkinName))
	{
		OutRoleSkinInfo = &AllRoleSkinInfo[SkinName];
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
/// Role Form Info
bool UGamePawnManager::GetRoleFormInfoFromName(const FName& FormName, const FRoleFormInfo* & OutRoleFormInfo) const
{
	if (AllRoleFormInfo.Contains(FormName))
	{
		OutRoleFormInfo = &AllRoleFormInfo[FormName];
		return true;
	}
	return false;
}



TArray<FName> UGamePawnManager::GetAllGamePawnFormWithRoleName(const FName& RoleName)
{
	TArray<FName> Result;
	for (TMap<FName, FRoleFormInfo>::TConstIterator It(AllRoleFormInfo); It; ++It)
	{
		if (It.Value().RoleName == RoleName)
		{
			Result.AddUnique(It.Key());
		}
	}
	return Result;
}




//////////////////////////////////////////////////////////////////////////
/// Role Display Info

bool UGamePawnManager::GetRoleDisplayInfoFromName(const FName& RoleName, FRoleDisplayInfo const*& OutRoleDisplayInfo)
{
	if (AllRoleDisplayInfo.Contains(RoleName))
	{
		OutRoleDisplayInfo = &AllRoleDisplayInfo[RoleName];
		return true;
	}


	if (RoleDisplayInfoDT)
	{
		FRoleDisplayInfo* RoleDisplayInfo = RoleDisplayInfoDT->FindRow<FRoleDisplayInfo>(RoleName, TEXT("-_- find role display info"));
		if (RoleDisplayInfo)
		{
			AllRoleDisplayInfo.Add(RoleName, *RoleDisplayInfo);
			OutRoleDisplayInfo = &AllRoleDisplayInfo[RoleName];
			return true;
		}
	}

	return false;
}





//////////////////////////////////////////////////////////////////////////
/// Skin and Form class info

FGamePawnSkin* UGamePawnManager::CreateGamePawnSkinFromName(const FName& Name, UStaticMeshComponent* InStaticMeshComp)
{
	if (AllGamePawnSkinClassInfo.Contains(Name))
	{
		return AllGamePawnSkinClassInfo[Name]->CreateObject(InStaticMeshComp);
	}
	return nullptr;
}

void UGamePawnManager::RegisterGamePawnSkinWithName(const FName& Name, FGamePawnSkinClassInfo* ClassInfo)
{
	AllGamePawnSkinClassInfo.Add(Name, ClassInfo);
}



FGamePawnForm* UGamePawnManager::CreateGamePawnFormFromName(const FName& Name, AGamePawn* InGamePawn)
{
	if (AllGamePawnFormClassInfo.Contains(Name))
	{
		return AllGamePawnFormClassInfo[Name]->CreateObject(InGamePawn);
	}
	return nullptr;
}

void UGamePawnManager::RegisterGamePawnFormWithName(const FName& Name, FGamePawnFormClassInfo* ClassInfo)
{
	AllGamePawnFormClassInfo.Add(Name, ClassInfo);
}