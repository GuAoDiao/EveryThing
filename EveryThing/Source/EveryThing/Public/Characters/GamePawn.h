// Fill out your copyright notice in the Description page of Project Settings.

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
protected:
	void ResetInfoFromDataTable(const FName& GamePawnName);
	void SetInfo(const FGamePawnInfo* InInfo);
	UFUNCTION()
	void OnRep_Info();

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_Info)
	FGamePawnInfo OwnerInfo;
	UPROPERTY(BlueprintReadOnly, Replicated)
	float Durability;
	UPROPERTY(BlueprintReadOnly)
	float MaxDurability;
	UPROPERTY(BlueprintReadOnly, Replicated)
	float Energy;
	UPROPERTY(BlueprintReadOnly)
	float MaxEnergy;
public:
	UFUNCTION(BlueprintPure)
	float GetDurability() const { return Durability; }	
	void ChangeEnergy(float value);
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerAddDurability(float value);
	UFUNCTION(BlueprintPure)
	float GetEnergy() const { return Energy; }
	void ChangeDurability(float value);
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerAddEnergy(float value);
	float GetMaxHyperopiaDistance() const { return OwnerInfo.MaxHyperopiaDistance; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateDurabilityDelegate, float /* Durability */);
	FOnUpdateDurabilityDelegate OnUpdateDurabilityDelegate;
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
	///
};