// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingPlayerState.h"

#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#include "Characters/GamePawn.h"

#include "Characters/Controlllers/JumpMovementPawnController.h"
#include "Characters/Controlllers/RotaryMovementPawnController.h"

#include "Characters/PlayerPawns/PlayerChairPawn.h"
#include "Characters/PlayerPawns/PlayerFootballPawn.h"

AEveryThingPlayerState::AEveryThingPlayerState()
{
	AllGamePawn.Add(APlayerChairPawn::StaticClass());
	AllGamePawn.Add(APlayerFootballPawn::StaticClass());
}

void AEveryThingPlayerState::ToggolePawn(int32 NumberIndex)
{
	if (AllGamePawn.IsValidIndex(NumberIndex))
	{
		APlayerController* OwnerPC = Cast<APlayerController>(GetOwner());
		APawn* OwnerPawn = OwnerPC->GetPawn();
		OwnerPawn->SetActorHiddenInGame(true);

		AGamePawn* NewPawn = GetWorld()->SpawnActor<AGamePawn>(AllGamePawn[NumberIndex], OwnerPawn->GetActorLocation(), OwnerPawn->GetActorRotation());
		OwnerPC->Possess(OwnerPawn);
		OwnerPawn->Destroy();
	}
}