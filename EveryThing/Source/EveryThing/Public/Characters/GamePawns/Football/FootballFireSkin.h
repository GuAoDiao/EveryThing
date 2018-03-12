// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Skin/GamePawnSkin.h"

/**
*
*/
class EVERYTHING_API FFootballFireSkin : public FGamePawnSkin
{
public:
	FFootballFireSkin(UStaticMeshComponent* StaticMeshComp);

	DECLARE_GAMEPAWNSKIN_CLASS("FootballFireSkin");
};
