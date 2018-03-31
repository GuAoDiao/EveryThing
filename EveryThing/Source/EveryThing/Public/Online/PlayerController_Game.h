// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Characters/GamePawnControllerInterface.h"
#include "ChatWindow/ChatWindowControllerInterface.h"

#include "PlayerController_Game.generated.h"

class UAttackComponent;
class USkillComponent;
class UChatComponent;

/**
 * 
 */
UCLASS()
class EVERYTHING_API APlayerController_Game : public APlayerController, public IGamePawnControllerInterface, public IChatWindowControllerInterface
{
	GENERATED_BODY()
	
public:
	APlayerController_Game();
	
	virtual void BeginPlay() override;	
	virtual void SetPawn(APawn* InPawn) override;
protected:
	void RebindInput();

	void RemoveActionAndAxisBindings(const TArray<FName>& BindingsName);
	void ResetAxisAndActionMapping();

	//////////////////////////////////////////////////////////////////////////
	/// Game flow path
public:
	UFUNCTION(Client, Reliable)
	void ClientWaitForHousePlayerLoad();
	UFUNCTION(Client, Reliable)
	void ClientReadyToStart();
	UFUNCTION(Client, Reliable)
	void ClientStartPlayer();
	UFUNCTION(Client, Reliable)
	void ClientGameOver(int32 Gold);

	//////////////////////////////////////////////////////////////////////////
	/// Player State
public:
	virtual void OnRep_PlayerState() override;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateUpdateDelegate, APlayerState* /* PlayerState */)
	FOnPlayerStateUpdateDelegate OnPlayerStateUpdateDelegate;

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
	const FName& GetCurrentRoleName() const { return CurrentRoleName; }
	
	void StartToggleRole();
	void StopToggleRole();
	
	void ToggleRoleWithIndex(int32 NumberIndex);
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnToggleToTargetRoleSuccessDelegate, const FName& /* TargetRoleName */);
	FOnToggleToTargetRoleSuccessDelegate OnRoleNameUpdateDelegate;
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnToggleToTargetRoleFailureDelegate, const FName& /* TargetRoleName */, const FText&  /* ErrorInfo */);
	FOnToggleToTargetRoleFailureDelegate OnToggleToTargetRoleFailureDelegate;
	
private:
	void ToggleRoleWithName(const FName& TargetRoleName);

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerToggleRole(const FName& TargetRoleName);

	UFUNCTION()
	void OnRep_CurrentRoleName() { OnCurrentRoleNameUpdate(); }
	void OnCurrentRoleNameUpdate() { OnRoleNameUpdateDelegate.Broadcast(CurrentRoleName); }
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentRoleName)
	FName CurrentRoleName;

	TArray<FName> AllRoleNames;
};
