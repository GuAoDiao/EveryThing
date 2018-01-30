// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackComponent.h"

UAttackComponent::UAttackComponent() : CommonAttack(this), SpecialAttack(this)
{
	CommonAttack.BindingName = TEXT("CommonAttack");
	SpecialAttack.BindingName = TEXT("SpecialAttack");
}

void UAttackComponent::RebindInputComp(UInputComponent* OwnerInputComp)
{
	Super::RebindInputComp(OwnerInputComp);
	
	CommonAttack.RebindInput(OwnerInputComp);
	SpecialAttack.RebindInput(OwnerInputComp);
}

void UAttackComponent::StartCommonAttack() {}
void UAttackComponent::StopCommonAttack() {}
void UAttackComponent::ExcuteCommonAttack(float AxisValue) {}

void UAttackComponent::StartSpecialAttack() {}
void UAttackComponent::StopSpecialAttack() {}
void UAttackComponent::ExcuteSpecialAttack(float AxisValue) {}


void UAttackComponent::RebindAll()
{
	CommonAttack.RebindFunction((FMoves::SkillednessAction)&UAttackComponent::StartCommonAttack, (FMoves::SkillednessAction)&UAttackComponent::StopCommonAttack, (FMoves::SkillednessAxis)&UAttackComponent::ExcuteCommonAttack);
	SpecialAttack.RebindFunction((FMoves::SkillednessAction)&UAttackComponent::StartSpecialAttack, (FMoves::SkillednessAction)&UAttackComponent::StopSpecialAttack, (FMoves::SkillednessAxis)&UAttackComponent::ExcuteSpecialAttack);
}