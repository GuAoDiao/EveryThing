// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GamePawn.h"
#include "Characters/Movement/Interfaces/JumpMovementPawnInterface.h"
#include "Components/StaticMeshComponent.h"

#include "JumpMovementPawn.generated.h"


UCLASS()
class EVERYTHING_API AJumpMovementPawn : public AGamePawn, public IJumpMovementPawnInterface
{
	GENERATED_BODY()

public:
	AJumpMovementPawn();
protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UJumpMovementComponent* OwnerJumpMovementComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UBoxComponent* BoxCollisionComp;
	//////////////////////////////////////////////////////////////////////////
	/// IJumpMovementInterface
public:
	/// For Controller
	virtual UJumpMovementComponent* GetJumpMovementComponent() const override { return OwnerJumpMovementComp; }
	/// For Rotary Movement
	virtual UPrimitiveComponent* GetPrimitiveComponent() const override { return StaticMeshComp; };

public:
	UFUNCTION()
	void OnBoxCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit);
};