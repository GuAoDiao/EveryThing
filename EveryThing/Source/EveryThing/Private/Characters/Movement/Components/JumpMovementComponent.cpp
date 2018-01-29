// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpMovementComponent.h"

#include "Components/PrimitiveComponent.h"
#include "UnrealNetwork.h"

#include "Characters/Movement/Interfaces/JumpMovementPawnInterface.h"

UJumpMovementComponent::UJumpMovementComponent()
{
	bReplicates = true;

	JumpForwardForce = 800 * 1000.f;
	JumpHeightForce = 600 * 1000.f;


	AdjustLocationForce = 500 * 1000.f;
	AdjustRotationForce = 300000 * 1000.f;

	AdjustPawnRotationForce = 500000 * 1000.f;

	AtuoAdjustRotationForceStrength = 100.f;

	bHasMoveDirection = false;
	bIsToogleMovementState = false;
	bCanJump = true;

	OwnerJumpPawn = Cast<IJumpMovementPawnInterface>(GetOwner());
	OwnerPrimitiveComp = OwnerJumpPawn ? OwnerJumpPawn->GetPrimitiveComponent() : nullptr;

	PrimaryComponentTick.bCanEverTick = true;
}

void UJumpMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (OwnerPrimitiveComp) { AutoAdjsutRotationPosition(DeltaTime); }
}

//////////////////////////////////////////////////////////////////////////
/// Adjust Position

void UJumpMovementComponent::AdjustForwardPosition(float AxisValue) { AdjustPosition(!bIsToogleMovementState, true, AxisValue); }
void UJumpMovementComponent::AdjustRightPosition(float AxisValue) { AdjustPosition(!bIsToogleMovementState, false, AxisValue); }

void UJumpMovementComponent::AutoAdjsutRotationPosition(float DeltaTime)
{
	if (OwnerPrimitiveComp)
	{
		FRotator OwnerRotation = OwnerPrimitiveComp->GetComponentRotation();
	
		FVector Force = FVector::ZeroVector;

		if (OwnerRotation.Roll > 10.f)
		{
			// Force += OwnerPrimitiveComp->GetRightVector() * FMath::FInterpTo(10, OwnerRotation.Roll, DeltaTime, AtuoAdjustRotationForceStrength);
		}
		else if(OwnerRotation.Roll < -10.f)
		{
			// Force += OwnerPrimitiveComp->GetRightVector() * FMath::FInterpTo(OwnerRotation.Roll , -10, DeltaTime, AtuoAdjustRotationForceStrength);
		}

		if (OwnerRotation.Pitch > 10.f)
		{
			// Force += OwnerPrimitiveComp->GetForwardVector() * FMath::FInterpTo(10, OwnerRotation.Pitch, DeltaTime, AtuoAdjustRotationForceStrength);
		}
		else if(OwnerRotation.Pitch < -10.f)
		{
			// Force += OwnerPrimitiveComp->GetForwardVector() * FMath::FInterpTo(OwnerRotation.Pitch, -10, DeltaTime, AtuoAdjustRotationForceStrength);
		}

		if (Force != FVector::ZeroVector)
		{
			OwnerPrimitiveComp->AddTorqueInRadians(Force * AdjustRotationForce  * DeltaTime);
		}
	}
}


void UJumpMovementComponent::AdjustPosition(bool bIsAdjsutLocation, bool bIsForward, float AxisValue)
{
	if (!HasAuthority())
	{
		ServerAdjustPosition(bIsAdjsutLocation, bIsForward, AxisValue);
	}
		
	if (OwnerPrimitiveComp)
	{
		if (bIsAdjsutLocation)
		{
			FVector Direction = bIsForward ? OwnerPrimitiveComp->GetForwardVector() : OwnerPrimitiveComp->GetRightVector();
			OwnerPrimitiveComp->AddForce(Direction * AdjustLocationForce * AxisValue);
		}
		else
		{
			FVector Direction = bIsForward ? OwnerPrimitiveComp->GetRightVector() : -OwnerPrimitiveComp->GetForwardVector();
			OwnerPrimitiveComp->AddTorqueInRadians(Direction * AdjustRotationForce * AxisValue);
		}
	}
}

bool UJumpMovementComponent::ServerAdjustPosition_Validate(bool bIsAdjsutLocation, bool bIsForward, float AxisValue) { return true; }
void UJumpMovementComponent::ServerAdjustPosition_Implementation(bool bIsAdjsutLocation, bool bIsForward, float AxisValue) { AdjustPosition(bIsAdjsutLocation, bIsForward, AxisValue); }

void UJumpMovementComponent::RotatePawn(float AxisValue)
{
	if (!HasAuthority())
	{
		ServerRotatePawn(AxisValue);
	}
	
	if (OwnerPrimitiveComp) { OwnerPrimitiveComp->AddTorqueInRadians(OwnerPrimitiveComp->GetUpVector() * AdjustPawnRotationForce * AxisValue); }
}
bool UJumpMovementComponent::ServerRotatePawn_Validate(float AxisValue) { return true; }
void UJumpMovementComponent::ServerRotatePawn_Implementation(float AxisValue) { RotatePawn(AxisValue); }


//////////////////////////////////////////////////////////////////////////
/// Jump And Jump Move

inline void UJumpMovementComponent::JumpMoveToForward() { SetJumpMoveDirection(true, true); }
inline void UJumpMovementComponent::JumpMoveToBack() { SetJumpMoveDirection(true, false); }
inline void UJumpMovementComponent::JumpMoveToRight() { SetJumpMoveDirection(false, true); }
inline void UJumpMovementComponent::JumpMoveToLeft() { SetJumpMoveDirection(false, false); }

void UJumpMovementComponent::SetJumpMoveDirection(bool bIsForward, bool bIsPositive)
{
	bHasMoveDirection = true;
	if (OwnerPrimitiveComp)
	{
		WantedMoveDirection = bIsForward ? OwnerPrimitiveComp->GetForwardVector() : OwnerPrimitiveComp->GetRightVector();

		if (!bIsPositive) { WantedMoveDirection = -WantedMoveDirection; }
	}
}

void UJumpMovementComponent::ClearMoveDirection()
{
	bHasMoveDirection = false;
}

void UJumpMovementComponent::StartJump()
{
	if (bHasMoveDirection)
	{
		JumpMove(WantedMoveDirection);
	}
	else
	{
		Jump();
	}
}
void UJumpMovementComponent::StopJump()
{
}


void UJumpMovementComponent::Jump()
{
	if (!HasAuthority())
	{
		ServerJump();
	}

	if (OwnerPrimitiveComp && bCanJump)
	{
		UE_LOG(LogTemp, Log, TEXT("-_- this is jump"));
		OwnerPrimitiveComp->AddImpulse(OwnerPrimitiveComp->GetUpVector() * JumpHeightForce);
		bCanJump = false;
	}
}
bool UJumpMovementComponent::ServerJump_Validate() { return true; }
void UJumpMovementComponent::ServerJump_Implementation() { Jump(); }


void UJumpMovementComponent::JumpMove(const FVector& Dircetion)
{
	if (!HasAuthority())
	{
		ServerJumpMove(Dircetion);
	}

	if (OwnerPrimitiveComp && bCanJump)
	{
		UE_LOG(LogTemp, Log, TEXT("-_- this is jump move"));
		OwnerPrimitiveComp->AddImpulse(OwnerPrimitiveComp->GetUpVector() * JumpHeightForce + WantedMoveDirection * JumpForwardForce);
		bCanJump = false;
	}
}
bool UJumpMovementComponent::ServerJumpMove_Validate(const FVector& Dircetion) { return true; }
void UJumpMovementComponent::ServerJumpMove_Implementation(const FVector& Dircetion) { JumpMove(Dircetion); }


void UJumpMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UJumpMovementComponent, bCanJump);

	DOREPLIFETIME(UJumpMovementComponent, JumpForwardForce);
	DOREPLIFETIME(UJumpMovementComponent, JumpHeightForce);

	DOREPLIFETIME(UJumpMovementComponent, AdjustLocationForce);
	DOREPLIFETIME(UJumpMovementComponent, AdjustRotationForce);
	DOREPLIFETIME(UJumpMovementComponent, AdjustPawnRotationForce);

	DOREPLIFETIME(UJumpMovementComponent, AtuoAdjustRotationForceStrength);
}