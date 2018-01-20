// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerPawnComponent.generated.h"

class IHitAbleInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVERYTHING_API UPlayerPawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerPawnComponent();
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	//////////////////////////////////////////////////////////////////////////
	/// Visual Angle
public:
	void Turn(float AxisValue);
	void LookUp(float AxisValue);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "VisualAngle")
	float BaseTurnRate;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "VisualAngle")
	float BaseLookUpRate;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "VisualAngle")
	bool bIsConvertYAxis;

	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Form And Prop Use
public:
	inline void SetIsWantedTogglePawnForm(bool bInIsWantedTogglePawnState) { bIsWantedTogglePawnState = bInIsWantedTogglePawnState; }
	void OnPressNumberKeyboard(int32 NumberIndex);

protected:
	void UseProp(int32 NumberIndex);
	void TogglePawnState(int32 NumberIndex);

	bool bIsWantedTogglePawnState;
	class AGamePawn* OwnerPawn;

	//////////////////////////////////////////////////////////////////////////
	/// Select Attack Target
public:
	AActor* GetAttackTarget() const { return CurrentAttackTarget; }
	
	void SelectNextAttackTarget();
	void SelectLastAttackTarget();

private:
	AActor* TryToGetHitAbleActor() const;
protected:
	AActor* LastAttackTarget;
	AActor* CurrentAttackTarget;
};