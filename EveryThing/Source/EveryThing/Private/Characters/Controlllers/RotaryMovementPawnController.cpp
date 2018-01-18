// Fill out your copyright notice in the Description page of Project Settings.

#include "RotaryMovementPawnController.h"

#include "Characters/Movement/Interfaces/RotaryMovementPawnInterface.h"
#include "Characters/Movement/Components/RotaryMovementComponent.h"

void ARotaryMovementPawnController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//////////////////////////////////////////////////////////////////////////
	/// Movement
	InputComponent->BindAxis("MoveForward", this, &ARotaryMovementPawnController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARotaryMovementPawnController::MoveRight);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ARotaryMovementPawnController::StartJump);
	InputComponent->BindAction("ToggleMovementState", IE_Pressed, this, &ARotaryMovementPawnController::ToogleMovementState);
}


void ARotaryMovementPawnController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	IRotaryMovementPawnInterface* OwnerRotaryPawn = Cast<IRotaryMovementPawnInterface>(InPawn);
	if (OwnerRotaryPawn)
	{
		OwnerRotaryMovementComp = OwnerRotaryPawn->GetRotaryMovementComponent();
	}
}

void ARotaryMovementPawnController::MoveForward(float AxisValue) { if (AxisValue != 0.f && OwnerRotaryMovementComp) { OwnerRotaryMovementComp->MoveForward(AxisValue); } }
void ARotaryMovementPawnController::MoveRight(float AxisValue) { if (AxisValue != 0.f && OwnerRotaryMovementComp) { OwnerRotaryMovementComp->MoveRight(AxisValue); } }
void ARotaryMovementPawnController::StartJump() { if (OwnerRotaryMovementComp) { OwnerRotaryMovementComp->StartJump(); } }
void ARotaryMovementPawnController::ToogleMovementState() { if (OwnerRotaryMovementComp) { OwnerRotaryMovementComp->ToogleMovementState(); } }