// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Skin/GamePawnSkin.h"

/**
 * 
 */
class EVERYTHING_API FChairBlueSkin : public FGamePawnSkin
{
public:
	FChairBlueSkin(UStaticMeshComponent* StaticMeshComp);

	DECLARE_GAMEPAWNSKIN_CLASS("ChairBlueSkin");
};
