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

UCLASS()
class EVERYTHING_API AGamePawn : public APawn, public IHitAbleInterface
{
	GENERATED_BODY()

public:
	AGamePawn();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//////////////////////////////////////////////////////////////////////////
	/// Component
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComp;

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
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleToNewPawnFormWithIndex(int32 Index);
private:
	// actual implementation
	void ToggleToNewPawnForm(FGamePawnForm* TargetGamePawnForm);

	FGamePawnForm* GetGamePawnForm(int32 Index);
	const TArray<FGamePawnForm*>& GetGamePawnForms() { return OwnerGamePawnForms; }

protected:
	FGamePawnForm* CurrentGamePawnForm;
	TArray<FGamePawnForm*> OwnerGamePawnForms;

	//////////////////////////////////////////////////////////////////////////
	/// Attack and Skill
public:
	AActor* TryToGetAttackTarget(float InMaxAttackDistance);

	inline UAttackComponent* GetAttackComponent() const { return OwnerAttackComp; }
	inline USkillComponent* GetSkillComponent() const { return OwnerSkillComp; }

	UFUNCTION(Server, Reliable, WithValidation)
	void ToggleToNewAttackComponent(UAttackComponent* InAttackComponent);
	UFUNCTION(Server, Reliable, WithValidation)
	void ToggleToNewSkillComponent(USkillComponent* InSkillComponent);
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UAttackComponent* OwnerAttackComp;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	USkillComponent* OwnerSkillComp;

	//////////////////////////////////////////////////////////////////////////
	/// Attribute
	
public:
	UFUNCTION(BlueprintPure)
	float GetDurability() const { return Durability; }
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void AddDurability(float InOffset, EElementType InType = EElementType::None);

	float GetMaxHyperopiaDistance() const { return MaxHyperopiaDistance; }

protected:
	// HP
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Attribute")
	float Durability;

	// _����(_����)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Attribute")
	float PhysicalPower;
	// _����(_�ٶ�)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Attribute")
	float Agility;
	// _�ȶ���(_����)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Attribute")
	float Stability;

	// _����ֵ(_����ֵ)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Attribute")
	float PowerValue;

	// _��Ұ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Attribute")
	float MaxHyperopiaDistance;

	//////////////////////////////////////////////////////////////////////////
	/// Element
	inline EElementType GetElementType() const { return ElementType; }

protected:
	// _Ԫ������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	EElementType ElementType;
	// _Ԫ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	float ElementValue;
	// _Ԫ�ؿ���
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	float ElementResistance;


	//////////////////////////////////////////////////////////////////////////
	/// Quality And Damping
protected:
	void ResetQuality();
	void ResetDamping();
	void ResetQualityAndDamping();

	inline void SetQualityScale(float InQualityScale) { QualityScale = InQualityScale; }
protected:
	// _����(_��������)
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute")
	float Quality;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	float QualityScale;

	// _����ֵ(_����)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	float LinearDamping;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	float AngularDamping;
};