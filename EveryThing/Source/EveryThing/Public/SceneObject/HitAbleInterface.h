// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HitAbleInterface.generated.h"

UINTERFACE()
class EVERYTHING_API UHitAbleInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class EVERYTHING_API IHitAbleInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual void SetIsSelectedToHit(bool bIsInSelectedToHit) = 0;
};
