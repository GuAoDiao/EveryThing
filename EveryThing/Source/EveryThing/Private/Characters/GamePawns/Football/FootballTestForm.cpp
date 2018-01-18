// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballTestForm.h"

#include "FootballAttackComponent.h"
#include "FootballTestSkillComponent.h"

#include "Characters/GamePawn.h"
#include "UObjectGlobals.h"


void FFootballTestForm::LoadGamePawnForm()
{
	if (OwnerGamePawn)
	{
		if (!OwnerGamePawn->IsActorInitialized())
		{
			OwnerAttackComp = OwnerGamePawn->CreateDefaultSubobject<UFootballAttackComponent>(TEXT("AttackComp"));
			OwnerSkillComp = OwnerGamePawn->CreateDefaultSubobject<UFootballTestSkillComponent>(TEXT("SkillComp"));
		}
		else
		{
			OwnerAttackComp = NewObject<UFootballAttackComponent>();
			OwnerSkillComp = NewObject<UFootballTestSkillComponent>();
		}
	}
}