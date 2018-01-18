// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerChairPawn.h"

#include "Characters/PlayerPawnComponent.h"

APlayerChairPawn::APlayerChairPawn()
{
	OwnerPlayerPawnComponent = CreateDefaultSubobject<UPlayerPawnComponent>(TEXT("PlayerPawnComponent"));
}

UPlayerPawnComponent* APlayerChairPawn::GetPlayerPawnComponent() const
{
	return OwnerPlayerPawnComponent;
}