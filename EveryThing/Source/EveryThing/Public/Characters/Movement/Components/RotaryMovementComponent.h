// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePawnMovementComponent.h"
#include "RotaryMovementComponent.generated.h"

class IHitAbleInterface;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EVERYTHING_API URotaryMovementComponent : public UGamePawnMovementComponent
{
	GENERATED_BODY()

public:
	URotaryMovementComponent();

	virtual void BindInputComponent(class UInputComponent* OwnerInputComp);

public:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Move(const FVector& Direction, float AxisValue);
	void MoveToLocation(const FVector& Location, float AxisValue);
	void AcceptForceImpulse(const FVector& Location, const FVector& Force);


	void StartJump();

	void ToogleMovementState();

	void SetIsJumping(bool bInIsJumping) { bIsJumping = bInIsJumping; }
	bool CanJump() { return !bIsJumping; }

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMove(const FVector& Direction, float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAcceptForceImpulse(const FVector& Location, const FVector& Force);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartJump();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToogleMovementState();
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float SpeedSlow;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float SpeedFast;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float JumpForceSmall;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float JumpForceStrong;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Transient, Replicated)
	float CurrentSpeed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Transient, Replicated)
	float CurrentJumpForce;


private:
	UPROPERTY(Replicated, Transient)
	bool bIsJumping;
	UPROPERTY(Replicated, Transient)
	bool bIsFastMovementState;

	class IRotaryMovementPawnInterface* OwnerRotaryPawn;
	class UPrimitiveComponent* OwnerPrimitiveComp;
};