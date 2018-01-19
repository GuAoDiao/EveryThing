// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Moves/AttackComponent.h"
#include "FootballAttackComponent.generated.h"

/**
*
*/
UCLASS()
class EVERYTHING_API UFootballAttackComponent : public UAttackComponent
{
	GENERATED_BODY()

public:
	UFootballAttackComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	//////////////////////////////////////////////////////////////////////////
	/// Hit
public:
	virtual void OnHitImplement(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit) override;
protected:
	UPROPERTY(BlueprintReadOnly ,EditDefaultsOnly)
	UParticleSystem* HitEmitter;

	virtual void StartCommonAttack() override;
	virtual void StartSpecialAttack() override;

private:
	AActor* TryToGetAttackTarget();

	void StartAttack(bool bInIsCommonAttack);
	void ToggleAttack(bool bInIsCommonAttack);
	void ExcuteAttack(float AxisValue);

	//////////////////////////////////////////////////////////////////////////
	/// Server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartAttack(bool bInIsCommonAttack, AActor* AttackTarget);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopAttack();
	
private:
	class AFootballPawn* OwnerPawn;
	UPROPERTY(Replicated)
	AActor* CurrentAttackTarget;
	UPROPERTY(Replicated)
	FVector LastAttackTartgetLocation;
	UPROPERTY(Replicated)
	bool bIsCommonAttack;
	UPROPERTY(Replicated)
	bool bIsAttacking;
	UPROPERTY(Replicated)
	bool bIsAutoAim;
	UPROPERTY(Replicated)
	float HitElasticScale;
	UPROPERTY(Replicated)
	float MinAutoAimDistance;
	UPROPERTY(Replicated)
	float MaxAttackDistance;
	UPROPERTY(Replicated)
	float RemainingTime;
};
