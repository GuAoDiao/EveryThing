// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePawnMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVERYTHING_API UGamePawnMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGamePawnMovementComponent();
	
	virtual void UpdateAgilityAndQuality(float Agility, float Quality, float QualityScale = 1.f);
protected:

	bool HasAuthority();

	float ActualMoveForce;
	float MoveForceScale;
	float ActualJumpForce;
	float JumpForceScale;
	float ActualSpeed;
	float SpeedScale;

public:
	virtual void RebindInputComp(class UInputComponent* OwnerInputComp) {}
};
