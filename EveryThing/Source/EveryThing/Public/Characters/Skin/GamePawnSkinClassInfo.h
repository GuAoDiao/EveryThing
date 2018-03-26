// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GamePawnManager.h"

class FGamePawnSkin;

class FGamePawnSkinClassInfo
{
public:
	FGamePawnSkinClassInfo(const FName& InName, GamePawnSkinCreateFunc InCreateFunc)
	{
		UGamePawnManager::RegisterGamePawnSkinWithName(InName, this);
		CreateFunc = InCreateFunc;
	}

	FGamePawnSkin* CreateObject(UStaticMeshComponent* InStaticMeshComp)
	{
		return CreateFunc ? (*CreateFunc)(InStaticMeshComp) : nullptr;
	}

	GamePawnSkinCreateFunc CreateFunc;
};