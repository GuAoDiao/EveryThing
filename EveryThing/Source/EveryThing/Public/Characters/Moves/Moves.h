// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UMovesComponent;

struct EVERYTHING_API FMoves
{
public:
	typedef void(UMovesComponent::*SkillednessAction)();
	typedef void(UMovesComponent::*SkillednessAxis)(float AxisValue);

	FName BindingName;

	bool bIsEnableActionPressed;
	bool bIsEnableActionReleased;
	bool bIsEnableAxis;

	UMovesComponent* SkillednessComp;
	
	void(UMovesComponent::*StartSkilledness)();
	void(UMovesComponent::*StopSkilledness)();
	void(UMovesComponent::*ExcuteSkilledness)(float AxisValue);

	FMoves()
	{
		bIsEnableAxis = bIsEnableActionPressed = bIsEnableActionReleased = false;
		SkillednessComp = nullptr;
		StartSkilledness = StopSkilledness = nullptr;
		ExcuteSkilledness = nullptr;
	}

	void RebindFunction(SkillednessAction InPressedFunc, SkillednessAction InReleasedFunc, SkillednessAxis InExcuteFunc)
	{
		StartSkilledness = bIsEnableActionPressed ? InPressedFunc : nullptr;
		StopSkilledness = bIsEnableActionReleased ? InReleasedFunc : nullptr;
		ExcuteSkilledness = bIsEnableAxis ? InExcuteFunc : nullptr;
	}
};