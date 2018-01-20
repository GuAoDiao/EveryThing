// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePawnMovementComponent.h"
#include "JumpMovementComponent.generated.h"

class IHitAbleInterface;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EVERYTHING_API UJumpMovementComponent : public UGamePawnMovementComponent
{
	GENERATED_BODY()

public:
	UJumpMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

protected:
	class IJumpMovementPawnInterface* OwnerJumpPawn;
	class UPrimitiveComponent* OwnerPrimitiveComp;
private:
	//////////////////////////////////////////////////////////////////////////
	/// Server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAdjustPosition(bool bIsAdjsutLocation, bool bIsForward, float AxisValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerJump();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerJumpMove(const FVector& Dircetion);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRotatePawn(float AxisValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAdjustPawnRotation(bool bNeedAdjustForward, bool bForwardIsPositiveValue, bool bNeedAdjustRight, bool bRightIsPositiveValue, float DeltaTime);

	//////////////////////////////////////////////////////////////////////////
	/// Adjust Position

public:
	void AdjustForwardPosition(float AxisValue);
	void AdjustRightPosition(float AxisValue);
	void AutoAdjsutRotationPosition(float DeltaTime);

private:
	void AdjustPosition(bool bIsAdjsutLocation, bool bIsForward, float AxisValue);
	void AdjsutRotationPosition(bool bNeedAdjustForward, bool bForwardIsPositiveValue, bool bNeedAdjustRight, bool bRightIsPositiveValue, float AxisValue);
	void RotatePawn(float AxisValue);

	//////////////////////////////////////////////////////////////////////////
	/// Jump And Jump Move
public:
	void SetJumpMoveDirection(bool bIsForward, bool bIsPositive);
	void Jump();
	void JumpMove(const FVector& Dircetion);

public:
	void JumpMoveToForward();
	void JumpMoveToBack();
	void JumpMoveToRight();
	void JumpMoveToLeft();

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
	float AdjustLocationForce;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated)
	float AdjustRotationForce;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated)
	float AdjustPawnRotationForce;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated)
	float AtuoAdjustRotationForceStrength;

	UPROPERTY(Replicated, Transient)
	bool bCanJump;
	
private:
	bool bIsToogleMovementState;
	bool bHasMoveDirection;
	FVector WantedMoveDirection;
};