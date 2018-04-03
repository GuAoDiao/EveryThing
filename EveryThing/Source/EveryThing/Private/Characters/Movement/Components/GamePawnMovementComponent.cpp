// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnMovementComponent.h"

#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Characters/GamePawn.h"

UGamePawnMovementComponent::UGamePawnMovementComponent()
{
	bReplicates = true;

	MoveForceScale = 25.f;
	SpeedScale = 5.f;
	JumpForceScale = 80.f;

	OwnerGamePawn = Cast<AGamePawn>(GetOwner());
}

bool UGamePawnMovementComponent::HasAuthority()
{
	AActor* Owner = GetOwner();
	return Owner ? Owner->HasAuthority() : false;
}

bool UGamePawnMovementComponent::IsAutonomousProxy()
{
	AActor* Owner = GetOwner();
	return Owner ? Owner->Role == ROLE_AutonomousProxy : false;
}

void UGamePawnMovementComponent::UpdateAgilityAndQuality(float Agility, float Quality, float QualityScale)
{
	ActualMoveForce = Quality * QualityScale * MoveForceScale;
	ActualJumpForce = Agility * Quality * QualityScale * JumpForceScale;
	ActualSpeed = Agility * SpeedScale;
}

bool UGamePawnMovementComponent::AddForceIfHaveEnoughStamina(const FVector& Force)
{
	if (OwnerGamePawn && OwnerPrimitiveComp)
	{
		if (OwnerGamePawn->CanConsumeForce(Force))
		{
			OwnerPrimitiveComp->AddForce(Force);
			OwnerGamePawn->OnConsumeForce(Force);
			return true;
		}
	}
	return false;
}

bool UGamePawnMovementComponent::AddForceAtLocationIfHaveEnoughStamina(const FVector& Force, const FVector& Location)
{
	if (OwnerGamePawn && OwnerPrimitiveComp)
	{
		if (OwnerGamePawn->CanConsumeForce(Force))
		{
			OwnerPrimitiveComp->AddForceAtLocation(Force, Location);
			OwnerGamePawn->OnConsumeForce(Force);
			return true;
		}
	}

	return false;
}

bool UGamePawnMovementComponent::AddTorqueInRadiansIfHaveEnoughStamina(const FVector& Torue)
{
	if (OwnerGamePawn && OwnerPrimitiveComp)
	{
		if (OwnerGamePawn->CanConsumeTorqueInRadians(Torue))
		{
			OwnerPrimitiveComp->AddTorqueInRadians(Torue);
			OwnerGamePawn->OnConsumeTorqueInRadians(Torue);
			return true;
		}
	}

	return false;
}

bool UGamePawnMovementComponent::AddImpulseIfHaveEnoughStamina(const FVector& Impulse)
{
	if (OwnerGamePawn && OwnerPrimitiveComp)
	{
		if (OwnerGamePawn->CanConsumeImpulse(Impulse))
		{
			OwnerPrimitiveComp->AddImpulse(Impulse);
			OwnerGamePawn->OnConsumeImpulse(Impulse);
			return true;
		}
	}
	return false;
}