// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GamePawnManager.h"

class FGamePawnForm;
class AGamePawn;

class FGamePawnFormClassInfo
{
public:
	FGamePawnFormClassInfo(const FName& InName, GamePawnFormCreateFunc InCreateFunc)
	{
		UGamePawnManager::RegisterGamePawnFormWithName(InName, this);
		ClassName = InName;
		CreateFunc = InCreateFunc;
	}

	FGamePawnForm* GetClass(AGamePawn* InGamePawn)
	{
		return CreateFunc ? (*CreateFunc)(InGamePawn) : nullptr;
	}

	FName ClassName;
	GamePawnFormCreateFunc CreateFunc;
};