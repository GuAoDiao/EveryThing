// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JumpMovementComponent.generated.h"

class IHitAbleInterface;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EVERYTHING_API UJumpMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UJumpMovementComponent();

private:
	//////////////////////////////////////////////////////////////////////////
	/// Server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAdjustPosition(bool bIsAdjsutLocation, bool bIsForward, float AxisValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerJump();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerJumpMove();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRotatePawn(float AxisValue);
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

public:
	void AdjustForwardPosition(float AxisValue);
	void AdjustRightPosition(float AxisValue);

	void RotatePawn(float AxisValue);


	void JumpMoveToForward();
	void JumpMoveToBack();
	void JumpMoveToRight();
	void JumpMoveToLeft();

	void SetJumpMoveDirection(bool bIsForward, bool bIsPositive);
	void ClearMoveDirection();

	void StartJump();
	void StopJump();

	void SetCanJump(bool bInCanJump) { bCanJump = bInCanJump; }

	void BeginToggleMovementState() { bIsToogleMovementState = true; }
	void EndToggleMovementState() { bIsToogleMovementState = false; }
	//////////////////////////////////////////////////////////////////////////
	/// Attribute
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated)
	float JumpForwardForce;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated)
	float JumpHeightForce;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated)
	float AutoAdjstRotationStrength;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated)
	float AdjustLocationForce;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated)
	float AdjustRotationForce;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated)
	float AdjustSelfRotationForce;

	UPROPERTY(Replicated)
	bool bWantToJumpMove;
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bHasMoveDirection;
	UPROPERTY(Replicated)
	bool bIsToogleMovementState;
	UPROPERTY(Replicated)
	bool bCanJump;
	UPROPERTY(Replicated)
	FVector CurrentMoveDirection;

	class IJumpMovementPawnInterface* OwnerJumpPawn;
	class UPrimitiveComponent* OwnerPrimitiveComp;
};