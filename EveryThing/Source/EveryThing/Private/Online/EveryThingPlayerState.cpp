// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingPlayerState.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"

#include "Characters/GamePawn.h"
#include "Characters/PlayerPawns/PlayerChairPawn.h"
#include "Characters/PlayerPawns/PlayerFootballPawn.h"

AEveryThingPlayerState::AEveryThingPlayerState()
{
	AllGamePawn.Add(APlayerChairPawn::StaticClass());
	AllGamePawn.Add(APlayerFootballPawn::StaticClass());
}

void AEveryThingPlayerState::BeginPlay()
{
	APlayerController* OwnerPC = Cast<APlayerController>(GetOwner());
	APawn* OwnerPawn = OwnerPC ? OwnerPC->GetPawn() : nullptr;
	
	CurrentPawnClass = OwnerPawn ? OwnerPawn->GetClass() : nullptr;
}

void AEveryThingPlayerState::ToggolePawn(int32 NumberIndex)
{
	if (AllGamePawn.IsValidIndex(NumberIndex) && AllGamePawn[NumberIndex] != CurrentPawnClass)
	{
		if (!HasAuthority())
		{
			ServerTogglePawn(NumberIndex);
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("-_- toggle pawn Of Index: %d"), NumberIndex)

		APlayerController* OwnerPC = Cast<APlayerController>(GetOwner());

		APawn* OwnerPawn = OwnerPC->GetPawn();

		OwnerPawn->SetActorEnableCollision(false);
		OwnerPawn->SetActorHiddenInGame(true);


		FVector Location = OwnerPawn->GetActorLocation();
		Location.Z += 100.f;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AGamePawn* NewPawn = GetWorld()->SpawnActor<AGamePawn>(AllGamePawn[NumberIndex], Location, OwnerPawn->GetActorForwardVector().Rotation());
		
		OwnerPC->Possess(NewPawn);

		OwnerPawn->Destroy();

		CurrentPawnClass = AllGamePawn[NumberIndex];
	}
}

bool AEveryThingPlayerState::ServerTogglePawn_Validate(int32 NumberIndex) { return true; }
void AEveryThingPlayerState::ServerTogglePawn_Implementation(int32 NumberIndex)
{
	ToggolePawn(NumberIndex);
}



void AEveryThingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingPlayerState, CurrentPawnClass);
}