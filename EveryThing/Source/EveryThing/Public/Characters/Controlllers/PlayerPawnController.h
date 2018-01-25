// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Characters/GamePawnControllerInterface.h"

#include "PlayerPawnController.generated.h"


struct FMoves;
class UAttackComponent;
class USkillComponent;

/**
 * 
 */
UCLASS()
class EVERYTHING_API APlayerPawnController : public APlayerController, public IGamePawnControllerInterface
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
	virtual void SetPawn(APawn* InPawn) override;
protected:
	void RemoveActionAndAxisBindings(const TArray<FName>& BindingsName);

	void ResetAxisAndActionMapping();

	//////////////////////////////////////////////////////////////////////////
	/// For IGamePawnControllerInterface
	virtual AActor* GetAttackTarget() override;

	//////////////////////////////////////////////////////////////////////////
	/// Visual Angle
public:
	void Turn(float AxisValue);
	void LookUp(float AxisValue);	
private:
	class AGamePawn* OwnerGamePawn;
	class UPlayerPawnComponent* OwnerPlayerPawnComp;

	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Form , Skin And Prop Use
	void StartTogglePawnState();
	void StopTogglePawnState();

	void StartTogglePawnSkin();
	void StopTogglePawnSkin();

	void NumberOne();
	void NumberTwo();
	void NumberThree();
	void NumberFour();

	void SelectNextAttackTarget();
	void SelectLastAttackTarget();

	//////////////////////////////////////////////////////////////////////////
	/// _Skilledness

	void StartSkilledness(FMoves* Skillness);
	void StopSkilledness(FMoves* Skillness);
	void ExcuteSkilledness(FMoves* Skillness, float Value);
	
	void BindSkillednessInputEvent(FMoves* Skillness, void(APlayerPawnController::*InStartKilledness)(), void(APlayerPawnController::*InStopKilledness)(), void(APlayerPawnController::*InExcuteKilledness)(float));

	// Attack
public:
	void StartCommonAttack();
	void StopCommonAttack();
	void ExcuteCommonAttack(float AxisValue);

	void StartSpecialAttack();
	void StopSpecialAttack();
	void ExcuteSpecialAttack(float AxisValue);
	
	void ToggleToNewAttackComponent(UAttackComponent* InAttackComp);
private:
	UAttackComponent* CurrentAttackComponent;
	FMoves* CommonAttack;
	FMoves* SpecialAttack;
	// Skill
public:
	void StartFirstSkill();
	void StopFirstSkill();
	void ExcuteFirstSkill(float AxisValue);

	void StartSecondSkill();
	void StopSecondSkill();
	void ExcuteSecondSkill(float AxisValue);

	void StartUltimateSkill();
	void StopUltimateSkill();
	void ExcuteUltimateSkill(float AxisValue);


	void ToggleToNewSkillComponent(USkillComponent* InSkillComp);

private:
	USkillComponent* CurrentSkillComponent;
	FMoves* FirstSkill;
	FMoves* SecondSkill;
	FMoves* UltimateSkill;
};
