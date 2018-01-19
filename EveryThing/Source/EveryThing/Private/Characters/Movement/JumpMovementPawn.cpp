// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpMovementPawn.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"


#include "Characters/Movement/Components/JumpMovementComponent.h"

AJumpMovementPawn::AJumpMovementPawn()
{
	OwnerJumpMovementComp = CreateDefaultSubobject<UJumpMovementComponent>(TEXT("JumpMovementComp"));


	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollisionComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	BoxCollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	BoxCollisionComp->SetCollisionResponseToAllChannels(ECR_Block);

	BoxCollisionComp->SetNotifyRigidBodyCollision(true);
	BoxCollisionComp->OnComponentHit.AddDynamic(this, &AJumpMovementPawn::OnBoxCompHit);

	BoxCollisionComp->SetupAttachment(GetRootComponent());
}

void AJumpMovementPawn::OnBoxCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	OwnerJumpMovementComp->SetCanJump(true);
}