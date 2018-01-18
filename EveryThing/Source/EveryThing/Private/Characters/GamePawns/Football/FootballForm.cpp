// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballForm.h"

#include "FootballAttackComponent.h"
#include "FootballSkillComponent.h"

#include "Characters/GamePawn.h"


void FFootballForm::LoadGamePawnForm()
{
	if (OwnerGamePawn)
	{
		if (!OwnerGamePawn->IsActorInitialized())
		{
			OwnerAttackComp = OwnerGamePawn->CreateDefaultSubobject<UFootballAttackComponent>(TEXT("AttackComp"));
			OwnerSkillComp = OwnerGamePawn->CreateDefaultSubobject<UFootballSkillComponent>(TEXT("SkillComp"));
		}
		else
		{
			OwnerAttackComp = NewObject<UFootballAttackComponent>();
			OwnerSkillComp = NewObject<UFootballSkillComponent>();
		}
	}
}