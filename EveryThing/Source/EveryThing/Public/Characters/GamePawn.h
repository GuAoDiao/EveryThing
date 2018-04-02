﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "EveryThingTypes.h"
#include "SceneObject/HitAbleInterface.h"

#include "GamePawn.generated.h"

class UAttackComponent;
class USkillComponent;
class FRoleForm;
class FRoleSkin;
class UGamePawnMovementComponent;

UCLASS()
class EVERYTHING_API AGamePawn : public APawn, public IHitAbleInterface
{
	GENERATED_BODY()

public:
	AGamePawn();

	virtual void OnConstruction(const FTransform& Transform) override;


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

	FName RoleName;

	//////////////////////////////////////////////////////////////////////////
	/// Possessed By

	virtual void PossessedBy(AController* NewController) override;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPossessedByControllerDelegate, AController* /* NewController */);
	FOnPossessedByControllerDelegate OnPossessedByControllerDelegate;

	//////////////////////////////////////////////////////////////////////////
	/// Component
public:
	inline UGamePawnMovementComponent* GetGamePawnMovementComponent() const { return MovementComp; }
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	class UGamePawnMovementComponent* MovementComp;

	//////////////////////////////////////////////////////////////////////////
	/// HitAble
public:
	virtual void SetIsSelectedToHit(bool bInIsSelectedToHit) override;
	virtual void AcceptHitFrom(AActor* OtherActor, FVector NormalInpulse, const FHitResult& Hit) override;
	//////////////////////////////////////////////////////////////////////////
	/// Hit
public:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit);
protected:
	virtual void OnHitImplement(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit);

public:
	DECLARE_MULTICAST_DELEGATE_FiveParams(FOnHitDelegate, UPrimitiveComponent* /* HitComp */, AActor* /* OtherActor */, UPrimitiveComponent* /* OtherComp */, FVector /* NormalInpulse */, const FHitResult& /* Hit */);
	FOnHitDelegate OnHitDelegate;


	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Form And Skin
protected:
	void ResetDefaultSkinAndFormFromDataTable();

	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Form
public:
	void ToggleToNewFormWithIndex(int32 Index);

	TArray<FName> AllRoleFormName;
	TArray<FName> AllHaveRoleFormName;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllHaveRoleFormNameUpdateDelegate, const TArray<FName>& /* AllHaveRoleSkinName */);
	FOnAllHaveRoleFormNameUpdateDelegate OnAllHaveRoleFormNameUpdateDelegate;

	void OnAllHaveRoleFormNameUpdate() { OnAllHaveRoleFormNameUpdateDelegate.Broadcast(AllHaveRoleFormName); }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnToggleToTargetFormSuccessDelegate, const FName& /* TargetFormName */);
	FOnToggleToTargetFormSuccessDelegate OnToggleToTargetFormSuccessDelegate;
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnToggleToTargetFormFailureDelegate, const FName& /* TargetFormName */, const FText&  /* ErrorInfo */);
	FOnToggleToTargetFormFailureDelegate OnToggleToTargetFormFailureDelegate;

protected:
	// actual implementation
	void ToggleToTargetForm(const FName& FormName);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleToTargetForm(const FName& FormName);
	UFUNCTION()
	void OnRep_CurrentRoleFormName();
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentRoleFormName)
	FName CurrentRoleFormName;
	FRoleForm* CurrentRoleForm;

	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Skin
public:
	void ToggleToNewSkinWithIndex(int32 Index);

	TArray<FName> AllRoleSkinName;
	TArray<FName> AllHaveRoleSkinName;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllHaveRoleSkinNameUpdateDelegate, const TArray<FName>& /* AllHaveRoleSkinName */);
	FOnAllHaveRoleSkinNameUpdateDelegate OnAllHaveRoleSkinNameUpdateDelegate;
	void OnAllHaveRoleSkinNameUpdate() { OnAllHaveRoleSkinNameUpdateDelegate.Broadcast(AllHaveRoleSkinName); }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnToggleToTargetSkinSuccessDelegate, const FName& /* TargetSkinName */);
	FOnToggleToTargetSkinSuccessDelegate OnToggleToTargetSkinSuccessDelegate;
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnToggleToTargetSkinFailureDelegate, const FName& /* TargetSkinName */, const FText&  /* ErrorInfo */);
	FOnToggleToTargetSkinFailureDelegate OnToggleToTargetSkinFailureDelegate;

protected:
	void ToggleToTargetSkin(const FName& SkinName);
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleToTargetSkin(const FName& SkinName);
	// actual implementation
	UFUNCTION()
	void OnRep_CurrentRoleSkinName();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentRoleSkinName)
	FName CurrentRoleSkinName;
	FRoleSkin* CurrentRoleSkin;

	//////////////////////////////////////////////////////////////////////////
	/// Attack and Skill
public:
	AActor* TryToGetAttackTarget(float InMaxAttackDistance);

	inline UAttackComponent* GetAttackComponent() const { return OwnerAttackComp; }
	inline USkillComponent* GetSkillComponent() const { return OwnerSkillComp; }

	void ToggleToNewAttackComponent(UAttackComponent* InAttackComponent);
	void ToggleToNewSkillComponent(USkillComponent* InSkillComponent);
protected:
	UFUNCTION()
	void OnRep_OwnerAttackComp();
	UFUNCTION()
	void OnRep_OwnerSkillComp();
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Replicated, ReplicatedUsing = OnRep_OwnerAttackComp)
	UAttackComponent* OwnerAttackComp;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Replicated, ReplicatedUsing = OnRep_OwnerSkillComp)
	USkillComponent* OwnerSkillComp;


	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn info
public:
	void UpdateInfo();

	float GetMaxHyperopiaDistance() const { return BaseInfo.MaxHyperopiaDistance; }
protected:
	void ResetInfoFromDataTable(const FName& GamePawnName);
	void SetInfo(const FGamePawnInfo* InInfo);
	UFUNCTION()
	void OnRep_BaseInfo();

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_BaseInfo)
	FGamePawnInfo BaseInfo;

	//////////////////////////////////////////////////////////////////////////
	/// Durability
public:
	UFUNCTION(BlueprintPure)
	float GetDurability() const { return Durability; }
	UFUNCTION(BlueprintPure)
	float GetMaxDurability() const { return MaxDurability; }
	void Healed(AActor* Curer, float HealingValue);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDurabilityUpdateDelegate, float /* Durability */);
	FOnDurabilityUpdateDelegate OnDurabilityUpdateDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxDurabilityUpdateDelegate, float /* MaxDurability */);
	FOnMaxDurabilityUpdateDelegate OnMaxDurabilityUpdateDelegate;
protected:
	void ChangeDurability(float DurabilityOffset);

	UFUNCTION()
	void OnRep_Durability() { OnDurabilityUpdate(); }
	void OnDurabilityUpdate() { OnDurabilityUpdateDelegate.Broadcast(Durability); }

	UFUNCTION()
	void OnRep_MaxDurability() { OnMaxDurabilityUpdate(); }
	void OnMaxDurabilityUpdate() { OnMaxDurabilityUpdateDelegate.Broadcast(MaxDurability); }

	void GamePawnDeath();
	AActor* DamageCauserActor;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Durability)
	float Durability;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxDurability)
	float MaxDurability;
	//////////////////////////////////////////////////////////////////////////
	/// Stamina
public:
	UFUNCTION(BlueprintPure)
	float GetStamina() const { return Stamina; }
	UFUNCTION(BlueprintPure)
	float GetMaxStamina() const { return MaxStamina; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaUpdateDelegate, float /* Stamina */);
	FOnStaminaUpdateDelegate OnStaminaUpdateDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaUpdateDelegate, float /* MaxStamina */);
	FOnStaminaUpdateDelegate OnMaxStaminaUpdateDelegate;
protected:
	void SpendStamina(float Value);
	void ChangeStaminaTick(float DeltaTime);

	UFUNCTION()
	void OnRep_Stamina() { OnStaminaUpdate(); }
	void OnStaminaUpdate() { OnStaminaUpdateDelegate.Broadcast(Stamina); }
	UFUNCTION()
	void OnRep_MaxStamina() { OnMaxStaminaUpdate(); }
	void OnMaxStaminaUpdate() { OnMaxStaminaUpdateDelegate.Broadcast(MaxStamina); }

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina)
	float Stamina;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina)
	float MaxStamina;
	UPROPERTY(BlueprintReadOnly)
	float StaminaRecoverRate;

protected:
	//////////////////////////////////////////////////////////////////////////
	/// Damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnHitableActorTakeDamageDelegate, AGamePawn*, AActor*, float);
	FOnHitableActorTakeDamageDelegate OnHitableActorTakeDamageDelegate;

	UFUNCTION()
	void OnRep_HitableActorTakeDamage(AActor* HitableActor, float Damage) { OnHitableActorTakeDamage(HitableActor, Damage); }
	void OnHitableActorTakeDamage(AActor* HitableActor, float Damage) { OnHitableActorTakeDamageDelegate.Broadcast(this, HitableActor, Damage); }


protected:
	//////////////////////////////////////////////////////////////////////////
	/// Element

protected:

	//////////////////////////////////////////////////////////////////////////
	/// Quality And Damping
protected:
	void ResetQuality();
	void ResetDamping();

	void SetQualityScale(float InQualityScale);

	//////////////////////////////////////////////////////////////////////////
	/// On Use Force 
public:

	void OnConsumeForce(const FVector& Force);
	void OnConsumeTorqueInRadians(const FVector& Torque);
	void OnConsumeImpulse(const FVector& Impulse);

	bool CanConsumeForce(const FVector& Force);
	bool CanConsumeTorqueInRadians(const FVector& Torque);
	bool CanConsumeImpulse(const FVector& Impulse);

protected:
	float ForceParam = 5000000.f;
};