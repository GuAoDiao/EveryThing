// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GamePawnForm.h"

class EVERYTHING_API FFootballTestForm : public FGamePawnForm
{
public:
	using FGamePawnForm::FGamePawnForm;

	virtual void LoadGamePawnForm() override;
};