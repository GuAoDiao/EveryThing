// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePawnMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVERYTHING_API UGamePawnMovementComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	bool HasAuthority();

public:
	virtual void BindInputComponent(class UInputComponent* OwnerInputComp) {}
};
