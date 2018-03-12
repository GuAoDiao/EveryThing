// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GamePawnForm.h"

class EVERYTHING_API FFootballTestForm : public FGamePawnForm
{
public:
	FFootballTestForm(AGamePawn* InGamePawn);

	DECLARE_GAMEPAWNFORM_CLASS("FootballTestForm");
};