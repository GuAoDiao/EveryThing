// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EveryThingTypes.h"
#include "GamePawnManager.generated.h"

class AGamePawn;

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
	
};
