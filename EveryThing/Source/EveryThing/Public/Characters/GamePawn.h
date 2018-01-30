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
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void Tick(float DeltaTime) override;

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
	void AddGamePawnForm(FGamePawnForm* InGamePawnForm);
	void ToggleToNewPawnForm(int32 Index);
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleToNewPawnForm(int32 Index);
	// actual implementation
	void ToggleToTargetPawnForm(FGamePawnForm* TargetGamePawnForm);
	UFUNCTION()
	void OnRep_CurrentGamePawnFormIndex();
public:
	FGamePawnForm* GetGamePawnForm(int32 Index);
	const TArray<FGamePawnForm*>& GetGamePawnForms() { return OwnerGamePawnForms; }
protected:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentGamePawnFormIndex)
	int32 CurrentGamePawnFormIndex;
	FGamePawnForm* CurrentGamePawnForm;
	TArray<FGamePawnForm*> OwnerGamePawnForms;

	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Skin
public:
	void AddGamePawnSkin(FGamePawnSkin* InGamePawnSkin);

	void ToggleToNewPawnSkin(int32 Index, bool bIsCauser = true);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleToNewPawnSkin(int32 Index);
	// actual implementation
	void ToggleToTargetPawnSkin(FGamePawnSkin* TargetGamePawnSkin);

	UFUNCTION()
	void OnRep_CurrentGamePawnSkinIndex();
public:
	FGamePawnSkin* GetGamePawnSkin(int32 Index);
	const TArray<FGamePawnSkin*>& GetGamePawnSkins() { return OwnerGamePawnSkins; }

protected:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentGamePawnSkinIndex)
	int32 CurrentGamePawnSkinIndex;
	FGamePawnSkin* CurrentGamePawnSkin;
	TArray<FGamePawnSkin*> OwnerGamePawnSkins;

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
protected:
	void ResetInfoFromDataTable(const FName& GamePawnName);
	void SetInfo(const FGamePawnInfo* InInfo);
	void UpdateInfo();
	UFUNCTION()
	void OnRep_Info();

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_Info)
	FGamePawnInfo OwnerInfo;
	//////////////////////////////////////////////////////////////////////////
	/// Attribute
	
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