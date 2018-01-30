// Fill out your copyright notice in the Description page of Project Settings.

#include "Moves.h"

#include "Components/InputComponent.h"
#include "Characters/Moves/MovesComponent.h"


FMoves::FMoves(UMovesComponent* InMovesComp)
{
	
	bIsEnableAxis = bIsEnableActionPressed = bIsEnableActionReleased = false;
	MovesComp = InMovesComp;
	StartSkilledness = StopSkilledness = nullptr;
	ExcuteSkilledness = nullptr;
}

void FMoves::RebindFunction(SkillednessAction InPressedFunc, SkillednessAction InReleasedFunc, SkillednessAxis InExcuteFunc)
{
	StartSkilledness = bIsEnableActionPressed ? InPressedFunc : nullptr;
	StopSkilledness = bIsEnableActionReleased ? InReleasedFunc : nullptr;
	ExcuteSkilledness = bIsEnableAxis ? InExcuteFunc : nullptr;
}

void FMoves::RebindInput(UInputComponent* OwnerInputComp)
{
	if (OwnerInputComp && MovesComp)
	{
		if (bIsEnableActionPressed) { OwnerInputComp->BindAction(BindingName, IE_Pressed, MovesComp, StartSkilledness); }
		if (bIsEnableActionReleased) { OwnerInputComp->BindAction(BindingName, IE_Released, MovesComp, StopSkilledness); }
		if (bIsEnableAxis) { OwnerInputComp->BindAxis(BindingName, MovesComp, ExcuteSkilledness); }
	}
}