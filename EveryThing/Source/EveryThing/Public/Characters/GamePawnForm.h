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

	inline UAttackComponent* GetAttackComponent() const { return OwnerAttackComp; }
	inline USkillComponent* GetSkillComponent() const { return OwnerSkillComp; }

	virtual void LoadGamePawnForm() {};
	void UnloadGamePawnForm();
protected:
	AGamePawn* OwnerGamePawn;

	UAttackComponent* OwnerAttackComp;
	USkillComponent* OwnerSkillComp;
};
