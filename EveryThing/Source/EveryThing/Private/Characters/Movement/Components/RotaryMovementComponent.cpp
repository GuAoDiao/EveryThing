// Fill out your copyright notice in the Description page of Project Settings.

#include "RotaryMovementComponent.h"

#include "UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

#include "Characters/GamePawn.h"
#include "Characters/Movement/Interfaces/RotaryMovementPawnInterface.h"



URotaryMovementComponent::URotaryMovementComponent()
{	
	bIsJumping = false;
	bIsFastMovementState = false;

	FastSpeedScale = 1.5f;
	FastJumpScale = 1.5f;
	
	OwnerRotaryPawn = Cast<IRotaryMovementPawnInterface>(GetOwner());
	OwnerPrimitiveComp = OwnerRotaryPawn ? OwnerRotaryPawn->GetPrimitiveComponent() : nullptr;

	bWantsToMove = false;
	WantMoveDirection = FVector::ZeroVector;
}


void URotaryMovementComponent::MoveTimerImplementation()
{
	if (bWantsToMove && OwnerGamePawn)
	{
		Move(WantMoveDirection.GetSafeNormal(), 2.f);

		bWantsToMove = false;

		WantMoveDirection = FVector::ZeroVector;
	}
	else
	{
		--EmptyMoveTimerTick;
		if (EmptyMoveTimerTick <= 0)
		{
			if (GetWorld())
			{
				GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
			}
		}
	}
}

void URotaryMovementComponent::RebindInputComp(class UInputComponent* OwnerInputComp)
{
	Super::RebindInputComp(OwnerInputComp);

	if (!OwnerInputComp) { return; }

	OwnerInputComp->BindAxis("MoveForward", this, &URotaryMovementComponent::MoveForward);
	OwnerInputComp->BindAxis("MoveRight", this, &URotaryMovementComponent::MoveRight);

	OwnerInputComp->BindAction("Jump", IE_Pressed, this, &URotaryMovementComponent::StartJump);
	OwnerInputComp->BindAction("ToggleMovementState", IE_Pressed, this, &URotaryMovementComponent::ToogleMovementState);
}

void URotaryMovementComponent::UpdateAgilityAndQuality(float Agility, float Quality, float QualityScale)
{
	Super::UpdateAgilityAndQuality(SpeedScale, Quality, QualityScale);

	SpeedForceBase = ActualMoveForce * ActualSpeed ;
	JumpForceBase = ActualJumpForce;

	SetMovementState(bIsFastMovementState);
}


void URotaryMovementComponent::MoveForward(float AxisValue)
{
	if (OwnerRotaryPawn && AxisValue!= 0.f)
	{
		bWantsToMove = true;

		FVector Direction = OwnerRotaryPawn->GetActualForwardVector();
		Direction.Z = 0.f;

		WantMoveDirection += AxisValue * Direction;

		if (GetWorld())
		{
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			EmptyMoveTimerTick = 10;
			if (!TimerManager.IsTimerActive(MoveTimer))
			{
				TimerManager.SetTimer(MoveTimer, this, &URotaryMovementComponent::MoveTimerImplementation, 0.03f, true);
			}
		}
	}
}

void URotaryMovementComponent::MoveRight(float AxisValue)
{
	if (OwnerRotaryPawn && AxisValue != 0.f)
	{
		bWantsToMove = true;

		FVector Direction = OwnerRotaryPawn->GetActualRightVector();
		Direction.Z = 0.f;

		WantMoveDirection += AxisValue * Direction;

		if (GetWorld())
		{
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			EmptyMoveTimerTick = 10;
			if (!TimerManager.IsTimerActive(MoveTimer))
			{
				TimerManager.SetTimer(MoveTimer, this, &URotaryMovementComponent::MoveTimerImplementation, 0.05f, true);
			}
		}
	}
}

void URotaryMovementComponent::Move(const FVector& Direction, float AxisValue)
{
	if (!HasAuthority() && IsAutonomousProxy())
	{
		ServerMove(Direction, AxisValue);
	}

	if (OwnerPrimitiveComp)
	{
		AddForceIfHaveEnoughStamina(Direction * CurrentSpeed * AxisValue);
	}
}

bool URotaryMovementComponent::ServerMove_Validate(const FVector& Direction, float AxisValue) { return true; }
void URotaryMovementComponent::ServerMove_Implementation(const FVector& Direction, float AxisValue) { Move(Direction, AxisValue); }

void URotaryMovementComponent::MoveToLocation(const FVector& Location, float AxisValue)
{
	if (OwnerPrimitiveComp) { Move(UKismetMathLibrary::FindLookAtRotation(OwnerPrimitiveComp->GetComponentLocation(), Location).Vector(), AxisValue); }
}

void URotaryMovementComponent::AcceptForceImpulse(const FVector& Location, const FVector& Force)
{
	if (!HasAuthority() && IsAutonomousProxy())
	{
		ServerAcceptForceImpulse(Location, Force);
	}

	if (OwnerPrimitiveComp) { OwnerPrimitiveComp->AddImpulseAtLocation(Force, Location); }
}
bool URotaryMovementComponent::ServerAcceptForceImpulse_Validate(const FVector& Location, const FVector& Force) { return true; }
void URotaryMovementComponent::ServerAcceptForceImpulse_Implementation(const FVector& Location, const FVector& Force) { AcceptForceImpulse(Location, Force); }

void URotaryMovementComponent::StartJump()
{
	if (!HasAuthority() && IsAutonomousProxy())
	{
		ServerStartJump();
	}

	if (CanJump() && OwnerRotaryPawn && OwnerPrimitiveComp)
	{
		if (AddImpulseIfHaveEnoughStamina(OwnerRotaryPawn->GetActualUpVector() * CurrentJumpForce))
		{
			SetIsJumping(true);
		}
	}
}
bool URotaryMovementComponent::ServerStartJump_Validate() { return true; }
void URotaryMovementComponent::ServerStartJump_Implementation() { StartJump(); }

void URotaryMovementComponent::ToogleMovementState()
{
	if (!HasAuthority() && IsAutonomousProxy())
	{
		ServerToogleMovementState();
	}

	bIsFastMovementState = !bIsFastMovementState;
	SetMovementState(bIsFastMovementState);
}

void URotaryMovementComponent::SetMovementState(bool bInIsFastMovementState)
{
	UE_LOG(LogTemp, Log, TEXT("-_- current is %s state"), bInIsFastMovementState ? TEXT("fast") : TEXT("slow"));

	if (bInIsFastMovementState)
	{
		CurrentSpeed = SpeedForceBase * FastSpeedScale;
		CurrentJumpForce = JumpForceBase * FastJumpScale;
	}
	else
	{
		CurrentSpeed = SpeedForceBase;
		CurrentJumpForce = JumpForceBase;
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