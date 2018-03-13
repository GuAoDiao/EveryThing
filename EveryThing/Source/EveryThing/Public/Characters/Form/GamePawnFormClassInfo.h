// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GamePawnManager.h"

class FGamePawnForm;
class AGamePawn;

class FGamePawnFormClassInfo
{
public:
	FGamePawnFormClassInfo(const FName& InName, const FName& InRoleName, int32 InCost ,GamePawnFormCreateFunc InCreateFunc)
	{
		UGamePawnManager::RegisterGamePawnFormWithName(InName, this);
		FormName = InName;
		RoleName = InRoleName;
		CreateFunc = InCreateFunc;
		Cost = InCost;
	}

	FGamePawnForm* GetClass(AGamePawn* InGamePawn)
	{
		return CreateFunc ? (*CreateFunc)(InGamePawn) : nullptr;
	}

	int32 Cost;
	FName FormName;
	FName RoleName;
	GamePawnFormCreateFunc CreateFunc;
};