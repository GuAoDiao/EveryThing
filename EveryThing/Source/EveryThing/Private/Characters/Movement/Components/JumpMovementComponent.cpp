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

	AutoAdjstRotationStrength = 25.f;

	AdjustLocationForce = 500 * 1000.f;
	AdjustRotationForce = 100000 * 1000.f;

	AdjustSelfRotationForce = 500000 * 1000.f;

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
		FRotator OwnerRotation = OwnerPrimitiveComp->GetComponentRotation();
		if (OwnerRotation.Roll > 10.f)
		{
			ServerAdjustPosition(false, false, -1.f * DeltaTime * AutoAdjstRotationStrength);
		} 
		else if (OwnerRotation.Roll < -10.f)
		{
			ServerAdjustPosition(false, false, 1.f * DeltaTime * AutoAdjstRotationStrength);
		}

		if (OwnerRotation.Pitch > 10.f)
		{
			ServerAdjustPosition(false, true, 1.f * DeltaTime * AutoAdjstRotationStrength);
		}
		else if (OwnerRotation.Pitch < -10.f)
		{
			ServerAdjustPosition(false, true, -1.f * DeltaTime * AutoAdjstRotationStrength);
		}


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

void UJumpMovementComponent::AdjustForwardPosition(float AxisValue)
{
	ServerAdjustPosition(!bIsToogleMovementState, true, AxisValue);
}
void UJumpMovementComponent::AdjustRightPosition(float AxisValue)
{
	ServerAdjustPosition(!bIsToogleMovementState, false, AxisValue);
}

bool UJumpMovementComponent::ServerAdjustPosition_Validate(bool bIsAdjsutLocation, bool bIsForward, float AxisValue) { return true; }
void UJumpMovementComponent::ServerAdjustPosition_Implementation(bool bIsAdjsutLocation, bool bIsForward, float AxisValue)
{
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

void UJumpMovementComponent::RotatePawn(float AxisValue)
{
	ServerRotatePawn(AxisValue);
}
bool UJumpMovementComponent::ServerRotatePawn_Validate(float AxisValue) { return true; }
void UJumpMovementComponent::ServerRotatePawn_Implementation(float AxisValue)
{
	if (OwnerPrimitiveComp)
	{
		OwnerPrimitiveComp->AddTorqueInRadians(OwnerPrimitiveComp->GetUpVector() * AdjustSelfRotationForce * AxisValue);
	}
}


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
		ServerJumpMove(WantedMoveDirection);
	}
	else
	{
		ServerJump();
	}
}
void UJumpMovementComponent::StopJump()
{
}

bool UJumpMovementComponent::ServerJump_Validate() { return true; }
void UJumpMovementComponent::ServerJump_Implementation()
{
	if (OwnerPrimitiveComp && bCanJump)
	{
		UE_LOG(LogTemp, Log, TEXT("-_- this is jump"));
		OwnerPrimitiveComp->AddImpulse(OwnerPrimitiveComp->GetUpVector() * JumpHeightForce);
		bCanJump = false;
	}
}

bool UJumpMovementComponent::ServerJumpMove_Validate(const FVector& Dircetion) { return true; }
void UJumpMovementComponent::ServerJumpMove_Implementation(const FVector& Dircetion)
{
	if (OwnerPrimitiveComp && bCanJump)
	{
		UE_LOG(LogTemp, Log, TEXT("-_- this is jump move"));
		OwnerPrimitiveComp->AddImpulse(OwnerPrimitiveComp->GetUpVector() * JumpHeightForce + WantedMoveDirection * JumpForwardForce);
		bCanJump = false;
	}
}


void UJumpMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UJumpMovementComponent, bCanJump);

	DOREPLIFETIME(UJumpMovementComponent, AutoAdjstRotationStrength);
	DOREPLIFETIME(UJumpMovementComponent, JumpForwardForce);
	DOREPLIFETIME(UJumpMovementComponent, JumpHeightForce);

	DOREPLIFETIME(UJumpMovementComponent, AdjustLocationForce);
	DOREPLIFETIME(UJumpMovementComponent, AdjustRotationForce);
	DOREPLIFETIME(UJumpMovementComponent, AdjustSelfRotationForce);
}