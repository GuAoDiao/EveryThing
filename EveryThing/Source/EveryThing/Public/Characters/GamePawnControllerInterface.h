// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePawnControllerInterface.generated.h"

UINTERFACE()
class EVERYTHING_API UGamePawnControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class EVERYTHING_API IGamePawnControllerInterface
{
	GENERATED_BODY()

public:
	virtual class AActor* GetAttackTarget() = 0;
};