// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnMovementComponent.h"

#include "GameFramework/Actor.h"

UGamePawnMovementComponent::UGamePawnMovementComponent()
{
	bReplicates = true;


	MoveForceScale = 10.f;
	SpeedScale = 10.f;
	JumpForceScale = 75.f;
}

bool UGamePawnMovementComponent::HasAuthority()
{
	AActor* Owner = GetOwner();
	return Owner ? Owner->HasAuthority() : false;
}

void UGamePawnMovementComponent::UpdateAgilityAndQuality(float Agility, float Quality, float QualityScale)
{
	ActualMoveForce = Quality * QualityScale * MoveForceScale;
	ActualJumpForce = Agility * Quality * QualityScale * JumpForceScale;
	ActualSpeed = Agility * SpeedScale;
}