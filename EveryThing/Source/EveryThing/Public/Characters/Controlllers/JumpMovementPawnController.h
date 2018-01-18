// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Controlllers/PlayerPawnController.h"
#include "JumpMovementPawnController.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API AJumpMovementPawnController : public APlayerPawnController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;

	virtual void SetPawn(APawn* InPawn) override;
private:
	class UJumpMovementComponent* OwnerJumpMovementComp;

public:
	void AdjsutForwardPosition(float AxisValue);
	void AdjustRightPosition(float AxisValue);


	void JumpMoveToForward();
	void JumpMoveToBack();
	void JumpMoveToLeft();
	void JumpMoveToRight();

	void ClearMoveDirection();

	void StartJump();
	void StopJump();

	void BeginToggleMovementState();
	void EndToggleMovementState();
};
