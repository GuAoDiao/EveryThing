// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Characters/GamePawnControllerInterface.h"
#include "ChatWindow/ChatWindowControllerInterface.h"

#include "PlayerPawnController.generated.h"

class UAttackComponent;
class USkillComponent;
class UChatComponent;

/**
 * 
 */
UCLASS()
class EVERYTHING_API APlayerPawnController : public APlayerController, public IGamePawnControllerInterface, public IChatWindowControllerInterface
{
	GENERATED_BODY()
	
public:
	APlayerPawnController();
	

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
	virtual void SetPawn(APawn* InPawn) override;
protected:
	void RebindInput();

	void RemoveActionAndAxisBindings(const TArray<FName>& BindingsName);
	void ResetAxisAndActionMapping();

	//////////////////////////////////////////////////////////////////////////
	/// For Game Pawn Controller Interface
	virtual AActor* GetAttackTarget() override;

	//////////////////////////////////////////////////////////////////////////
	/// For Chat Window Controller Interface
public:
	virtual UChatComponent* GetChatComponent() const { return ChatComponent; }

private:
	UChatComponent* ChatComponent;

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
	void FocusToChatWindow();

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

	void ToggleRoleWithIndex(int32 NumberIndex);



	DECLARE_MULTICAST_DELEGATE_OneParam(FOnToggleToTargetRoleSuccessDelegate, const FName& /* TargetRoleName */);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnToggleToTargetRoleFailureDelegate, const FName& /* TargetRoleName */, const FText&  /* ErrorInfo */);

	FOnToggleToTargetRoleSuccessDelegate& GetOnToggleToTargetRoleSuccessDelegate() { return OnToggleToTargetRoleSuccessDelegate; }
	FOnToggleToTargetRoleFailureDelegate& GetOnToggleToTargetRoleFailureDelegate() { return OnToggleToTargetRoleFailureDelegate; }
private:
	FOnToggleToTargetRoleSuccessDelegate OnToggleToTargetRoleSuccessDelegate;
	FOnToggleToTargetRoleFailureDelegate OnToggleToTargetRoleFailureDelegate;
	
	void ToggleRoleWithName(const FName& TargetRoleName);

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerToggleRole(const FName& TargetRoleName);

	UPROPERTY(Transient, Replicated)
	FName CurrentRoleName;
};
