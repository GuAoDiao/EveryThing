// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballTestForm.h"

#include "FootballAttackComponent.h"
#include "FootballTestSkillComponent.h"

 FFootballTestForm::FFootballTestForm(AGamePawn* InGamePawn) : FGamePawnForm(InGamePawn)
{
	AttackCompClass = UFootballAttackComponent::StaticClass();
	SkillCompClass = UFootballTestSkillComponent::StaticClass();
}