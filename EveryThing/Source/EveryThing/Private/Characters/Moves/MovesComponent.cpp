// Fill out your copyright notice in the Description page of Project Settings.

#include "MovesComponent.h"

#include "GameFramework/Actor.h"

UMovesComponent::UMovesComponent()
{
	bReplicates = true;
}

bool UMovesComponent::HasAuthority()
{
	AActor* Owner = GetOwner();
	return Owner ? Owner->HasAuthority() : false;
}