// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpMovementComponent.h"

#include "Components/PrimitiveComponent.h"
#include "UnrealNetwork.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include "Characters/Movement/Interfaces/JumpMovementPawnInterface.h"

UJumpMovementComponent::UJumpMovementComponent()
{
	bReplicates = true;

	JumpForwardForce = 800 * 1000.f;
	JumpHeightForce = 600 * 1000.f;


	AdjustLocationForce = 500 * 1000.f;
	AdjustRotationForce = 100000 * 1000.f;

	AdjustPawnRotationForce = 500000 * 1000.f;

	AtuoAdjustRotationForceStrength = 25.f;

	bHasMoveDirection = false;
	bIsToogleMovementState = false;
	bCanJump = true;

	OwnerJumpPawn = Cast<IJumpMovementPawnInterface>(GetOwner());
	OwnerPrimitiveComp = OwnerJumpPawn ? OwnerJumpPawn->GetPrimitiveComponent() : nullptr;

	PrimaryComponentTick.bCanEverTick = true;
}

void UJumpMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (OwnerPrimitiveComp)
	{
		AutoAdjsutRotationPosition(DeltaTime);
		

		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		APlayerController* OwnerPC = OwnerPawn ? Cast<APlayerController>(OwnerPawn->GetController()) : nullptr;
		
		if (OwnerPC)
		{
			int32 Width, Height;
			OwnerPC->GetViewportSize(Width, Height);

			FVector OwnerDirection, CenterDirection;
			OwnerPC->DeprojectScreenPositionToWorld(Width * 0.5f, Height * 0.5f, OwnerDirection, CenterDirection);
			OwnerDirection = OwnerPrimitiveComp->GetForwardVector();
			
			CenterDirection.Z = 0.f; OwnerDirection.Z = 0.f;

			float Value = FVector::CrossProduct(OwnerDirection, CenterDirection).Z;
			if (FMath::Abs(Value) > 0.1f)
			{
				Value = Value > 0.f ? FMath::Clamp(Value, 0.2f, 1.f) : FMath::Clamp(Value, -1.f, -0.2f);
				RotatePawn(Value * DeltaTime * 100.f);
			}
		}
	}
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
	
		bool bNeedAdjustForward = OwnerRotation.Roll > 10.f || OwnerRotation.Roll < -10.f;
		bool bForwardIsPositiveValue = OwnerRotation.Roll > 0.f;

		bool bNeedAdjustRight = OwnerRotation.Pitch > 10.f || OwnerRotation.Pitch < -10.f;
		bool bRightIsPositiveValue = OwnerRotation.Pitch > 0.f;

		if (bNeedAdjustForward || bNeedAdjustRight)
		{
			AdjsutRotationPosition(bNeedAdjustForward, bForwardIsPositiveValue, bNeedAdjustRight, bRightIsPositiveValue, DeltaTime);
		}
	}
}

void UJumpMovementComponent::AdjsutRotationPosition(bool bNeedAdjustForward, bool bForwardIsPositiveValue, bool bNeedAdjustRight, bool bRightIsPositiveValue, float AxisValue)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		ServerAdjustPawnRotation(bNeedAdjustForward, bForwardIsPositiveValue, bNeedAdjustRight, bRightIsPositiveValue, AxisValue);
		return;
	}

	if (bNeedAdjustForward) { AdjustPosition(false, false, (bForwardIsPositiveValue ? 1.f : -1.f) * AxisValue * AtuoAdjustRotationForceStrength); }
	if (bNeedAdjustRight) { AdjustPosition(false, true, (bRightIsPositiveValue ? 1.f : -1.f) * AxisValue * AtuoAdjustRotationForceStrength); }
}
bool UJumpMovementComponent::ServerAdjustPawnRotation_Validate(bool bNeedAdjustForward, bool bForwardIsPositiveValue, bool bNeedAdjustRight, bool bRightIsPositiveValue, float AxisValue) { return true; }
void UJumpMovementComponent::ServerAdjustPawnRotation_Implementation(bool bNeedAdjustForward, bool bForwardIsPositiveValue, bool bNeedAdjustRight, bool bRightIsPositiveValue, float AxisValue)
{
	AdjsutRotationPosition(bNeedAdjustForward, bForwardIsPositiveValue, bNeedAdjustRight, bRightIsPositiveValue, AxisValue);
}



void UJumpMovementComponent::AdjustPosition(bool bIsAdjsutLocation, bool bIsForward, float AxisValue)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		ServerAdjustPosition(bIsAdjsutLocation, bIsForward, AxisValue);
		return;
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
	if (GetOwnerRole() != ROLE_Authority)
	{
		ServerRotatePawn(AxisValue);
		return;
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
	if (GetOwnerRole() != ROLE_Authority)
	{
		ServerJump();
		return;
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
	if (GetOwnerRole() != ROLE_Authority)
	{
		ServerJumpMove(Dircetion);
		return;
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