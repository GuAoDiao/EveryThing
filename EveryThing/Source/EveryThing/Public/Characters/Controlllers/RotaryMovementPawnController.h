// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Controlllers/PlayerPawnController.h"
#include "RotaryMovementPawnController.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API ARotaryMovementPawnController : public APlayerPawnController
{
	GENERATED_BODY()
	
	
public:
	virtual void SetupInputComponent() override;

	virtual void SetPawn(APawn* InPawn) override;
private:
	class URotaryMovementComponent* OwnerRotaryMovementComp;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void StartJump();
	void ToogleMovementState();
};