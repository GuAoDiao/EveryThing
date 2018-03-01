// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Characters/GamePawnControllerInterface.h"

#include "PlayerPawnController.generated.h"

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
	void RebindInput();

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
	/// UI
	void DisplayGameMenu();

	//////////////////////////////////////////////////////////////////////////
	/// Attack and skill
public:	
	void ToggleToNewAttackComponent(UAttackComponent* InAttackComp);
	void ToggleToNewSkillComponent(USkillComponent* InSkillComp);

	//////////////////////////////////////////////////////////////////////////
	/// Toggle Role

public:
	void StartToggleRole();
	void StopToggleRole();

	bool bIsWantedTogglePawn;

	void ToggoleRole(int32 NumberIndex);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnToggleToTargetRoleSuccessDelegate, const FName& /* TargetRoleName */);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnToggleToTargetRoleFailureDelegate, const FName& /* TargetRoleName */, const FText&  /* ErrorInfo */);

	FOnToggleToTargetRoleSuccessDelegate& GetOnToggleToTargetRoleSuccessDelegate() { return OnToggleToTargetRoleSuccessDelegate; }
	FOnToggleToTargetRoleFailureDelegate& GetOnToggleToTargetRoleFailureDelegate() { return OnToggleToTargetRoleFailureDelegate; }
private:
	FOnToggleToTargetRoleSuccessDelegate OnToggleToTargetRoleSuccessDelegate;
	FOnToggleToTargetRoleFailureDelegate OnToggleToTargetRoleFailureDelegate;
	
	UFUNCTION(Server, WithValidation, Reliable)
	void ServerToggleRole(int32 NumberIndex);

	UPROPERTY(Transient, Replicated)
	FName CurrentRoleName;
};
