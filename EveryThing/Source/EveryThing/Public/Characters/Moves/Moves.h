// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UMovesComponent;
class UInputComponent;

struct EVERYTHING_API FMoves
{
public:
	typedef void(UMovesComponent::*SkillednessAction)();
	typedef void(UMovesComponent::*SkillednessAxis)(float AxisValue);

	FMoves(UMovesComponent* InMovesComp = nullptr);
private:
	UMovesComponent* MovesComp;
public:
	void RebindFunction(SkillednessAction InPressedFunc, SkillednessAction InReleasedFunc, SkillednessAxis InExcuteFunc);
	
	void RebindInput(UInputComponent* OwnerInputComp);
private:

public:
	FName BindingName;

	bool bIsEnableActionPressed;
	bool bIsEnableActionReleased;
	bool bIsEnableAxis;
	SkillednessAction StartSkilledness;
	SkillednessAction StopSkilledness;
	SkillednessAxis ExcuteSkilledness;
};