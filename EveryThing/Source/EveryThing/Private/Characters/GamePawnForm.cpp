// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnForm.h"

#include "Characters/Moves/SkillComponent.h"
#include "Characters/Moves/AttackComponent.h"

FGamePawnForm::FGamePawnForm(AGamePawn* InGamePawn)
{
	OwnerGamePawn = InGamePawn;

	OwnerAttackComp = nullptr;
	OwnerSkillComp = nullptr;
}

void FGamePawnForm::UnloadGamePawnForm()
{
	if (OwnerAttackComp) { OwnerAttackComp->DestroyComponent(); }
	if (OwnerSkillComp) { OwnerSkillComp->DestroyComponent(); }
}