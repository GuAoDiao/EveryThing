// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GamePawnManager.h"

class FGamePawnSkin;

class FGamePawnSkinClassInfo
{
public:
	FGamePawnSkinClassInfo(const FName& InName, const FName& InRoleName, int32 InCost, GamePawnSkinCreateFunc InCreateFunc)
	{
		UGamePawnManager::RegisterGamePawnSkinWithName(InName, this);
		SkinName = InName;
		RoleName = InRoleName;
		CreateFunc = InCreateFunc;
		Cost = InCost;
	}

	FGamePawnSkin* GetClass(UStaticMeshComponent* InStaticMeshComp)
	{
		return CreateFunc ? (*CreateFunc)(InStaticMeshComp) : nullptr;
	}

	FName SkinName;
	FName RoleName;
	int32 Cost;
	GamePawnSkinCreateFunc CreateFunc;
};