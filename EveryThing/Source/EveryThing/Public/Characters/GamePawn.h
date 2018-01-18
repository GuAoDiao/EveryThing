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

	// _体力(_耐力)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Attribute")
	float PhysicalPower;
	// _敏捷(_速度)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Attribute")
	float Agility;
	// _稳定性(_防御)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Attribute")
	float Stability;

	// _蓄力值(_能量值)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Attribute")
	float PowerValue;

	// _视野距离
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Attribute")
	float MaxHyperopiaDistance;

	//////////////////////////////////////////////////////////////////////////
	/// Element
	inline EElementType GetElementType() const { return ElementType; }

protected:
	// _元素类型
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	EElementType ElementType;
	// _元素量
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	float ElementValue;
	// _元素抗性
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
	// _质量(_隐藏属性)
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute")
	float Quality;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	float QualityScale;

	// _阻力值(_阻尼)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	float LinearDamping;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	float AngularDamping;
};