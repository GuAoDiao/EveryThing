// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballForm.h"

#include "FootballAttackComponent.h"
#include "FootballSkillComponent.h"

IMPLEMENT_GAMEPAWNFORM_CLASS("FootballForm", "Football", 100, FFootballForm);

FFootballForm::FFootballForm(AGamePawn* InGamePawn) : FGamePawnForm(InGamePawn)
{
	AttackCompClass = UFootballAttackComponent::StaticClass();
	SkillCompClass = UFootballSkillComponent::StaticClass();
}