// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GamePawnSkin.h"

/**
 * 
 */
class EVERYTHING_API FFootballSkin : public FGamePawnSkin
{
public:
	using FGamePawnSkin::FGamePawnSkin;

	virtual void LoadGamePawnSkin() override;
};
