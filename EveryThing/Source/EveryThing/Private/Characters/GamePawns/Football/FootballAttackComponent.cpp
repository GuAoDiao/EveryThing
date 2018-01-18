// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballAttackComponent.h"

#include "UnrealNetwork.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/GamePawns/Football/FootballPawn.h"
#include "Characters/Movement/Components/RotaryMovementComponent.h"

UFootballAttackComponent::UFootballAttackComponent()
{
	CommonAttack.bIsEnableActionPressed = true;
	SpecialAttack.bIsEnableActionPressed = true;
	
	RebindAll();


	OwnerPawn = Cast<AFootballPawn>(GetOwner());
	
	bIsAttacking = false;
	CurrentAttackTarget = nullptr;

	HitEmitter = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	
	bIsAutoAim = true;
	MinAutoAimDistance = 500.f;
	MaxAttackDistance = 5000.f;
	HitElasticScale = 0.3f;

	PrimaryComponentTick.bCanEverTick = true;
}

void UFootballAttackComponent::BeginPlay()
{
	PrimaryComponentTick.SetTickFunctionEnable(false);
}

void UFootballAttackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Log, TEXT("-_- this is current tick"));

	if (bIsAttacking)
	{
		ExcuteAttack(DeltaTime);
	}
	else
	{
		ServerStopAttack();
	}
}


void UFootballAttackComponent::OnHitImplement(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	Super::OnHitImplement(HitComp, OtherActor, OtherComp, NormalInpulse, Hit);
	
	if (Cast<IHitAbleInterface>(OtherActor))
	{
		if (bIsCommonAttack && OwnerPawn && OwnerPawn->GetRotaryMovementComponent())
		{
			OwnerPawn->GetRotaryMovementComponent()->Move(Hit.Normal, HitElasticScale * OwnerPawn->GetVelocity().Size());
		}

		if (HitEmitter)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitEmitter, Hit.Location);
		}

		ServerStopAttack();
	}
}

void UFootballAttackComponent::StartCommonAttack() { StartAttack(true); }
void UFootballAttackComponent::StartSpecialAttack() { StartAttack(false); }


void UFootballAttackComponent::StartAttack(bool bInIsCommonAttack)
{
	if (bIsAttacking) { return; }

	AActor* AttackTarget = OwnerPawn ? OwnerPawn->TryToGetAttackTarget(MaxAttackDistance) : nullptr;

	if (AttackTarget != nullptr)
	{
		ServerStartAttack(bInIsCommonAttack, AttackTarget);
	}
}

bool UFootballAttackComponent::ServerStartAttack_Validate(bool bInIsCommonAttack, AActor* AttackTarget) { return true; }
void UFootballAttackComponent::ServerStartAttack_Implementation(bool bInIsCommonAttack, AActor* AttackTarget)
{
	if (AttackTarget != nullptr)
	{
		bIsAttacking = true;
		CurrentAttackTarget = AttackTarget;
		bWantedToAcceptHitFunction = true;
		bIsCommonAttack = bInIsCommonAttack;
		bIsAutoAim = true;
		PrimaryComponentTick.SetTickFunctionEnable(true);
	}
}

bool UFootballAttackComponent::ServerStopAttack_Validate() { return true; }
void UFootballAttackComponent::ServerStopAttack_Implementation()
{
	bIsAttacking = false;
	bWantedToAcceptHitFunction = false;
	CurrentAttackTarget = nullptr;
	PrimaryComponentTick.SetTickFunctionEnable(false);
}

void UFootballAttackComponent::ExcuteAttack(float DeltaTime)
{
	URotaryMovementComponent* OwnerPawnRMC = OwnerPawn ? OwnerPawn->GetRotaryMovementComponent() : nullptr;

	if (CurrentAttackTarget && OwnerPawnRMC)
	{
		FVector AttackActorLocation;
		if (bIsAutoAim)
		{
			AttackActorLocation = CurrentAttackTarget->GetActorLocation();

			float Distance = (OwnerPawn->GetActorLocation() - AttackActorLocation).Size();
			if (Distance < MinAutoAimDistance)
			{
				bIsAutoAim = false;
				LastAttackTartgetLocation = AttackActorLocation;
				RemainingTime = Distance / OwnerPawn->GetVelocity().Size();
			}

		}
		else
		{
			AttackActorLocation = LastAttackTartgetLocation;

			RemainingTime -= DeltaTime;
			if (RemainingTime < 0.f)
			{
				ServerStopAttack();
			}
		}
		OwnerPawnRMC->MoveToLocation(AttackActorLocation, 5 * 100.f * DeltaTime);
	}
	else
	{
		ServerStopAttack();
	}
}


void UFootballAttackComponent::ToggleAttack(bool bInIsCommonAttack)
{
	if (bIsAttacking)
	{
		StartAttack(bInIsCommonAttack);
	}
	else
	{
		ServerStopAttack();
	}
}


void UFootballAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UFootballAttackComponent, CurrentAttackTarget);
	DOREPLIFETIME(UFootballAttackComponent, LastAttackTartgetLocation);
	DOREPLIFETIME(UFootballAttackComponent, bIsCommonAttack);
	DOREPLIFETIME(UFootballAttackComponent, bIsAttacking);
	DOREPLIFETIME(UFootballAttackComponent, bIsAutoAim);
	DOREPLIFETIME(UFootballAttackComponent, HitElasticScale);
	DOREPLIFETIME(UFootballAttackComponent, MinAutoAimDistance);
	DOREPLIFETIME(UFootballAttackComponent, MaxAttackDistance);
}