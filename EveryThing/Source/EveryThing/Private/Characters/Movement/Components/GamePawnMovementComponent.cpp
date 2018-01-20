// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnMovementComponent.h"

#include "GameFramework/Actor.h"

bool UGamePawnMovementComponent::HasAuthority()
{
	AActor* Owner = GetOwner();
	return Owner ? Owner->HasAuthority() : false;
}