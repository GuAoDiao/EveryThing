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
	/// Input
	void RebindInputComp(class UInputComponent* OwnerInputComp);
	
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
	void OnPressNumberKeyboard(int32 NumberIndex);

protected:
	void UseProp(int32 NumberIndex);
	void TogglePawn(int32 NumberIndex);
	void TogglePawnForm(int32 NumberIndex);
	void TogglePawnSkin(int32 NumberIndex);

	bool bIsWantedTogglePawn;
	bool bIsWantedTogglePawnSkin;
	bool bIsWantedTogglePawnForm;
	class AGamePawn* OwnerPawn;

	/// For input component
	void StartTogglePawn() { bIsWantedTogglePawn = true; }
	void StopTogglePawn() { bIsWantedTogglePawn = false; }

	void StartTogglePawnForm() { bIsWantedTogglePawnForm = true; }
	void StopTogglePawnForm() { bIsWantedTogglePawnForm = false; }

	void StartTogglePawnSkin() { bIsWantedTogglePawnSkin = true; }
	void StopTogglePawnSkin() { bIsWantedTogglePawnSkin = false; }

	void NumberOne() { OnPressNumberKeyboard(0); }
	void NumberTwo() { OnPressNumberKeyboard(1); }
	void NumberThree() { OnPressNumberKeyboard(2); }
	void NumberFour() { OnPressNumberKeyboard(3); }
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