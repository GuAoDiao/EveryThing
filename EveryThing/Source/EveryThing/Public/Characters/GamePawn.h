// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "EveryThingTypes.h"
#include "SceneObject/HitAbleInterface.h"

#include "GamePawn.generated.h"

class UAttackComponent;
class USkillComponent;
class FGamePawnForm;
class FGamePawnSkin;
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

	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Form
public:
	void ToggleToNewFormWithIndex(int32 Index);

	TArray<FName> AllGamePawnFormName;
	TArray<FName> AllHaveGamePawnFormName;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnToggleToTargetFormSuccessDelegate, const FName& /* TargetFormName */);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnToggleToTargetFormFailureDelegate, const FName& /* TargetFormName */, const FText&  /* ErrorInfo */);

	FOnToggleToTargetFormSuccessDelegate& GetOnToggleToTargetFormSuccessDelegate() { return OnToggleToTargetFormSuccessDelegate; }
	FOnToggleToTargetFormFailureDelegate& GetOnToggleToTargetFormFailureDelegate() { return OnToggleToTargetFormFailureDelegate; }
private:
	FOnToggleToTargetFormSuccessDelegate OnToggleToTargetFormSuccessDelegate;
	FOnToggleToTargetFormFailureDelegate OnToggleToTargetFormFailureDelegate;

protected:
	// actual implementation
	void ToggleToTargetForm(const FName& FormName);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleToTargetForm(const FName& FormName);
	UFUNCTION()
	void OnRep_CurrentGamePawnFormName();
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentGamePawnFormName)
	FName CurrentGamePawnFormName;
	FGamePawnForm* CurrentGamePawnForm;

	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Skin
public:
	void ToggleToNewSkinWithIndex(int32 Index);

	TArray<FName> AllGamePawnSkinName;
	TArray<FName> AllHaveGamePawnSkinName;
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnToggleToTargetSkinSuccessDelegate, const FName& /* TargetSkinName */);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnToggleToTargetSkinFailureDelegate, const FName& /* TargetSkinName */, const FText&  /* ErrorInfo */);

	FOnToggleToTargetSkinSuccessDelegate& GetOnToggleToTargetSkinSuccessDelegate() { return OnToggleToTargetSkinSuccessDelegate; }
	FOnToggleToTargetSkinFailureDelegate& GetOnToggleToTargetSkinFailureDelegate() { return OnToggleToTargetSkinFailureDelegate; }
private:
	FOnToggleToTargetSkinSuccessDelegate OnToggleToTargetSkinSuccessDelegate;
	FOnToggleToTargetSkinFailureDelegate OnToggleToTargetSkinFailureDelegate;

protected:
	void ToggleToTargetSkin(const FName& SkinName);
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleToTargetSkin(const FName& SkinName);
	// actual implementation
	UFUNCTION()
	void OnRep_CurrentGamePawnSkinName();
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentGamePawnSkinName)
	FName CurrentGamePawnSkinName;
	FGamePawnSkin* CurrentGamePawnSkin;
	
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
public:
	UFUNCTION(BlueprintPure)
	float GetDurability() const { return OwnerInfo.Durability; }
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void AddDurability(float InOffset, EElementType InType = EElementType::None);

	float GetMaxHyperopiaDistance() const { return OwnerInfo.MaxHyperopiaDistance; }

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

};