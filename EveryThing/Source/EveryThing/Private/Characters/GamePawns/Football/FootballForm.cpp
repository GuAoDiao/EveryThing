// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballForm.h"

#include "FootballAttackComponent.h"
#include "FootballSkillComponent.h"

FFootballForm::FFootballForm(AGamePawn* InGamePawn) : FGamePawnForm(InGamePawn)
{
	AttackCompClass = UFootballAttackComponent::StaticClass();
	SkillCompClass = UFootballSkillComponent::StaticClass();
}