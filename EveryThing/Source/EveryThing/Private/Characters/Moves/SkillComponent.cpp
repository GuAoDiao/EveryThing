// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillComponent.h"

USkillComponent::USkillComponent() : FirstSkill(this), SecondSkill(this), UltimateSkill(this)
{
	FirstSkill.BindingName = TEXT("FirstSkill");
	SecondSkill.BindingName = TEXT("SecondSkill");
	UltimateSkill.BindingName = TEXT("UltimateSkill");
}


void USkillComponent::RebindInputComp(UInputComponent* OwnerInputComp)
{
	Super::RebindInputComp(OwnerInputComp);

	FirstSkill.RebindInput(OwnerInputComp);
	SecondSkill.RebindInput(OwnerInputComp);
	UltimateSkill.RebindInput(OwnerInputComp);
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
	FirstSkill.RebindFunction((FMoves::SkillednessAction)&USkillComponent::StartFirstSkill, (FMoves::SkillednessAction)&USkillComponent::StopFirstSkill, (FMoves::SkillednessAxis)&USkillComponent::ExcuteFirstSkill);
	SecondSkill.RebindFunction((FMoves::SkillednessAction)&USkillComponent::StartSecondSkill, (FMoves::SkillednessAction)&USkillComponent::StopSecondSkill, (FMoves::SkillednessAxis)&USkillComponent::ExcuteSecondSkill);
	UltimateSkill.RebindFunction((FMoves::SkillednessAction)&USkillComponent::StartUltimateSkill, (FMoves::SkillednessAction) &USkillComponent::StopUltimateSkill, (FMoves::SkillednessAxis)&USkillComponent::ExcuteUltimateSkill);
}
