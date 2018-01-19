// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpMovementPawnController.h"

#include "GameFramework/Pawn.h"

#include "Characters/Movement/Interfaces/JumpMovementPawnInterface.h"
#include "Characters/Movement/Components/JumpMovementComponent.h"

void AJumpMovementPawnController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//////////////////////////////////////////////////////////////////////////
	/// Movement
	InputComponent->BindAxis("MoveForward", this, &AJumpMovementPawnController::AdjsutForwardPosition);
	InputComponent->BindAxis("MoveRight", this, &AJumpMovementPawnController::AdjustRightPosition);


	InputComponent->BindAction("MoveForwardAction", IE_Pressed, this, &AJumpMovementPawnController::JumpMoveToForward);
	InputComponent->BindAction("MoveBackAction", IE_Pressed, this, &AJumpMovementPawnController::JumpMoveToBack);
	InputComponent->BindAction("MoveLeftAction", IE_Pressed, this, &AJumpMovementPawnController::JumpMoveToLeft);
	InputComponent->BindAction("MoveRightAction", IE_Pressed, this, &AJumpMovementPawnController::JumpMoveToRight);

	InputComponent->BindAction("MoveForwardAction", IE_Released, this, &AJumpMovementPawnController::ClearMoveDirection);
	InputComponent->BindAction("MoveBackAction", IE_Released, this, &AJumpMovementPawnController::ClearMoveDirection);
	InputComponent->BindAction("MoveLeftAction", IE_Released, this, &AJumpMovementPawnController::ClearMoveDirection);
	InputComponent->BindAction("MoveRightAction", IE_Released, this, &AJumpMovementPawnController::ClearMoveDirection);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AJumpMovementPawnController::StartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AJumpMovementPawnController::StopJump);
	
	InputComponent->BindAction("ToggleMovementState", IE_Pressed, this, &AJumpMovementPawnController::BeginToggleMovementState);
	InputComponent->BindAction("ToggleMovementState", IE_Released, this, &AJumpMovementPawnController::EndToggleMovementState);
}


void AJumpMovementPawnController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	IJumpMovementPawnInterface* OwnerJumpPawn = Cast<IJumpMovementPawnInterface>(InPawn);
	OwnerJumpMovementComp = OwnerJumpPawn ? OwnerJumpPawn->GetJumpMovementComponent() : nullptr;
}


void AJumpMovementPawnController::AdjsutForwardPosition(float AxisValue) { if (OwnerJumpMovementComp) { OwnerJumpMovementComp->AdjustForwardPosition(AxisValue); } }
void AJumpMovementPawnController::AdjustRightPosition(float AxisValue) { if (OwnerJumpMovementComp) { OwnerJumpMovementComp->AdjustRightPosition(AxisValue); } }

void AJumpMovementPawnController::JumpMoveToForward() { if (OwnerJumpMovementComp) { OwnerJumpMovementComp->JumpMoveToForward(); } }
void AJumpMovementPawnController::JumpMoveToBack() { if (OwnerJumpMovementComp) { OwnerJumpMovementComp->JumpMoveToBack(); } }
void AJumpMovementPawnController::JumpMoveToLeft() { if (OwnerJumpMovementComp) { OwnerJumpMovementComp->JumpMoveToLeft(); } }
void AJumpMovementPawnController::JumpMoveToRight() { if (OwnerJumpMovementComp) { OwnerJumpMovementComp->JumpMoveToRight(); } }
	 
void AJumpMovementPawnController::ClearMoveDirection() { if (OwnerJumpMovementComp) { OwnerJumpMovementComp->ClearMoveDirection(); } }
	 
void AJumpMovementPawnController::StartJump() { if (OwnerJumpMovementComp) { OwnerJumpMovementComp->StartJump(); } }
void AJumpMovementPawnController::StopJump() { if (OwnerJumpMovementComp) { OwnerJumpMovementComp->StopJump(); } }
	 
void AJumpMovementPawnController::BeginToggleMovementState() { if (OwnerJumpMovementComp) { OwnerJumpMovementComp->BeginToggleMovementState(); } }
void AJumpMovementPawnController::EndToggleMovementState() { if (OwnerJumpMovementComp) { OwnerJumpMovementComp->EndToggleMovementState(); } }