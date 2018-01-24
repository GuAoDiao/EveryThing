// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AGamePawn;
class USkillComponent;
class UAttackComponent;

class EVERYTHING_API FGamePawnForm
{	
public:
	FGamePawnForm(AGamePawn* InGamePawn);

	virtual void LoadGamePawnForm();
	void UnloadGamePawnForm();
protected:
	AGamePawn* OwnerGamePawn;

	UClass* AttackCompClass;
	UClass* SkillCompClass;
};
