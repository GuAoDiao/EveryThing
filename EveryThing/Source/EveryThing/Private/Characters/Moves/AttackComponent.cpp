// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackComponent.h"


UAttackComponent::UAttackComponent()
{
	CommonAttack.BindingName = TEXT("CommonAttack");
	SpecialAttack.BindingName = TEXT("SpecialAttack");


	CommonAttack.SkillednessComp = SpecialAttack.SkillednessComp = this;
}


void UAttackComponent::StartCommonAttack() {}
void UAttackComponent::StopCommonAttack() {}
void UAttackComponent::ExcuteCommonAttack(float AxisValue) {}

void UAttackComponent::StartSpecialAttack() {}
void UAttackComponent::StopSpecialAttack() {}
void UAttackComponent::ExcuteSpecialAttack(float AxisValue) {}

void UAttackComponent::RebindCommonAttack()
{
	CommonAttack.RebindFunction((void(UMovesComponent::*)())&UAttackComponent::StartCommonAttack, (void(UMovesComponent::*)())&UAttackComponent::StopCommonAttack, (void(UMovesComponent::*)(float))&UAttackComponent::ExcuteCommonAttack);
}

void UAttackComponent::RebindSpecialAttack()
{
	SpecialAttack.RebindFunction((void(UMovesComponent::*)())&UAttackComponent::StartSpecialAttack, (void(UMovesComponent::*)())&UAttackComponent::StopSpecialAttack, (void(UMovesComponent::*)(float))&UAttackComponent::ExcuteSpecialAttack);
}

void UAttackComponent::RebindAll()
{
	RebindCommonAttack();
	RebindSpecialAttack();
}