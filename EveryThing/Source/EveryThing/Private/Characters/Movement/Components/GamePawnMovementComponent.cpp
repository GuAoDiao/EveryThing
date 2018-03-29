// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnMovementComponent.h"

#include "GameFramework/Actor.h"
#include "Characters/GamePawn.h"

UGamePawnMovementComponent::UGamePawnMovementComponent()
{
	bReplicates = true;

	MoveForceScale = 10.f;
	SpeedScale = 10.f;
	JumpForceScale = 75.f;

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