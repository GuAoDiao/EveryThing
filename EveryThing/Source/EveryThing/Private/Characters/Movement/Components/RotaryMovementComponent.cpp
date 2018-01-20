// Fill out your copyright notice in the Description page of Project Settings.

#include "RotaryMovementComponent.h"

#include "UnrealNetwork.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Characters/Movement/Interfaces/RotaryMovementPawnInterface.h"



URotaryMovementComponent::URotaryMovementComponent()
{
	bReplicates = true;

	SpeedSlow = 625 * 1000.f;
	SpeedFast = 1250 * 1000.f;

	JumpForceSmall = 275 * 1000.f;
	JumpForceStrong = 450 * 1000.f;

	CurrentSpeed = SpeedSlow;
	CurrentJumpForce = JumpForceSmall;

	bIsJumping = false;
	bIsFastMovementState = false;

	OwnerRotaryPawn = Cast<IRotaryMovementPawnInterface>(GetOwner());
	OwnerPrimitiveComp = OwnerRotaryPawn ? OwnerRotaryPawn->GetPrimitiveComponent() : nullptr;
}



void URotaryMovementComponent::MoveForward(float AxisValue)
{
	if (OwnerRotaryPawn)
	{
		FVector Direction = OwnerRotaryPawn->GetActualForwardVector();
		Direction.Z = 0.f;
		Move(Direction, AxisValue);
	}
}

void URotaryMovementComponent::MoveRight(float AxisValue)
{
	if (OwnerRotaryPawn)
	{
		FVector Direction = OwnerRotaryPawn->GetActualRightVector();
		Direction.Z = 0.f;
		Move(Direction, AxisValue);
	}
}

void URotaryMovementComponent::Move(const FVector& Direction, float AxisValue)
{
	if (!HasAuthority())
	{
		ServerMove(Direction, AxisValue);
	}

	if (OwnerPrimitiveComp) { OwnerPrimitiveComp->AddForce(Direction * CurrentSpeed * AxisValue); }
}
bool URotaryMovementComponent::ServerMove_Validate(const FVector& Direction, float AxisValue) { return true; }
void URotaryMovementComponent::ServerMove_Implementation(const FVector& Direction, float AxisValue) { Move(Direction, AxisValue); }

void URotaryMovementComponent::MoveToLocation(const FVector& Location, float AxisValue)
{
	if (OwnerPrimitiveComp) { Move(UKismetMathLibrary::FindLookAtRotation(OwnerPrimitiveComp->GetComponentLocation(), Location).Vector(), AxisValue); }
}

void URotaryMovementComponent::AcceptForceImpulse(const FVector& Location, const FVector& Force)
{
	if (!HasAuthority())
	{
		ServerAcceptForceImpulse(Location, Force);
	}

	if (OwnerPrimitiveComp) { OwnerPrimitiveComp->AddImpulseAtLocation(Force, Location); }
}
bool URotaryMovementComponent::ServerAcceptForceImpulse_Validate(const FVector& Location, const FVector& Force) { return true; }
void URotaryMovementComponent::ServerAcceptForceImpulse_Implementation(const FVector& Location, const FVector& Force) { AcceptForceImpulse(Location, Force); }

void URotaryMovementComponent::StartJump()
{
	if (!HasAuthority())
	{
		ServerStartJump();
	}

	if (CanJump() && OwnerRotaryPawn && OwnerPrimitiveComp)
	{
		OwnerPrimitiveComp->AddImpulse(OwnerRotaryPawn->GetActualUpVector() * CurrentJumpForce);
		SetIsJumping(true);
	}
}
bool URotaryMovementComponent::ServerStartJump_Validate() { return true; }
void URotaryMovementComponent::ServerStartJump_Implementation() { StartJump(); }

void URotaryMovementComponent::ToogleMovementState()
{
	if (!HasAuthority())
	{
		ServerToogleMovementState();
	}


	bIsFastMovementState = !bIsFastMovementState;
	if (bIsFastMovementState)
	{
		CurrentSpeed = SpeedFast;
		CurrentJumpForce = JumpForceStrong;
	}
	else
	{
		CurrentSpeed = SpeedSlow;
		CurrentJumpForce = JumpForceSmall;
	}
}
bool URotaryMovementComponent::ServerToogleMovementState_Validate() { return true; }
void URotaryMovementComponent::ServerToogleMovementState_Implementation() { ToogleMovementState(); }

void URotaryMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URotaryMovementComponent, CurrentSpeed);
	DOREPLIFETIME(URotaryMovementComponent, CurrentJumpForce);

	

	DOREPLIFETIME(URotaryMovementComponent, bIsJumping);
	DOREPLIFETIME(URotaryMovementComponent, bIsFastMovementState);
}