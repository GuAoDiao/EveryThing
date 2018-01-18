// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillComponent.h"

USkillComponent::USkillComponent()
{
	FirstSkill.BindingName = TEXT("FirstSkill");
	SecondSkill.BindingName = TEXT("SecondSkill");
	UltimateSkill.BindingName = TEXT("UltimateSkill");

	FirstSkill.SkillednessComp = SecondSkill.SkillednessComp = UltimateSkill.SkillednessComp = this;
}

void USkillComponent::StartFirstSkill() {}
void USkillComponent::StopFirstSkill() {}
void USkillComponent::ExcuteFirstSkill(float AxisValue) {}

void USkillComponent::StartSecondSkill() {}
void USkillComponent::StopSecondSkill() {}
void USkillComponent::ExcuteSecondSkill(float AxisValue) {}

void USkillComponent::StartUltimateSkill() {}
void USkillComponent::StopUltimateSkill() {}
void USkillComponent::ExcuteUltimateSkill(float AxisValue) {}


void USkillComponent::RebindAll()
{
	RebindFirstSkill();
	RebindSecondSkill();
	RebindUltimateSkill();
}

void USkillComponent::RebindFirstSkill()
{
	FirstSkill.RebindFunction((void(UMovesComponent::*)())&USkillComponent::StartFirstSkill, (void(UMovesComponent::*)())&USkillComponent::StopFirstSkill, (void(UMovesComponent::*)(float))&USkillComponent::ExcuteFirstSkill);
}
void USkillComponent::RebindSecondSkill()
{
	SecondSkill.RebindFunction((void(UMovesComponent::*)())&USkillComponent::StartSecondSkill, (void(UMovesComponent::*)())&USkillComponent::StopSecondSkill, (void(UMovesComponent::*)(float))&USkillComponent::ExcuteSecondSkill);
}
void USkillComponent::RebindUltimateSkill()
{
	UltimateSkill.RebindFunction((void(UMovesComponent::*)())&USkillComponent::StartUltimateSkill, (void(UMovesComponent::*)()) &USkillComponent::StopUltimateSkill, (void(UMovesComponent::*)(float))&USkillComponent::ExcuteUltimateSkill);
}