// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawnComponent.h"

#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Characters/GamePawn.h"

UPlayerPawnComponent::UPlayerPawnComponent()
{
	OwnerPawn = Cast<AGamePawn>(GetOwner());

	PrimaryComponentTick.bCanEverTick = false;

	if (OwnerPawn)
	{
		class USpringArmComponent* SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
		SpringArmComp->bUsePawnControlRotation = true;
		SpringArmComp->TargetArmLength = 500;
		SpringArmComp->SocketOffset = FVector(0.f, 0.f, 1000.f);

		SpringArmComp->bInheritPitch = true;
		SpringArmComp->bInheritRoll = true;
		SpringArmComp->bInheritRoll = true;
		
		SpringArmComp->SetupAttachment(OwnerPawn->GetRootComponent());

		class UCameraComponent* CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
		CameraComp->SetupAttachment(SpringArmComp);
		CameraComp->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	
		PrimaryComponentTick.bCanEverTick = true;
	}

	/// Visual Angle
	BaseTurnRate = 1.f;
	BaseLookUpRate = 1.f;
	bIsConvertYAxis = true;


	CurrentAttackTarget = nullptr;
	LastAttackTarget = nullptr;
}

void UPlayerPawnComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (OwnerPawn)
	{
		AActor* SelectedHitAbleActor = TryToGetHitAbleActor();

		if (SelectedHitAbleActor && SelectedHitAbleActor != CurrentAttackTarget)
		{
			IHitAbleInterface* CurrentHitableAttackTarget;
			if (CurrentAttackTarget)
			{
				CurrentHitableAttackTarget = Cast<IHitAbleInterface>(CurrentAttackTarget);
				if (CurrentHitableAttackTarget) { CurrentHitableAttackTarget->SetIsSelectedToHit(false); }
			}
			
			LastAttackTarget = CurrentAttackTarget;
			CurrentAttackTarget = SelectedHitAbleActor;

			CurrentHitableAttackTarget = Cast<IHitAbleInterface>(CurrentAttackTarget);
			if (CurrentHitableAttackTarget) { CurrentHitableAttackTarget->SetIsSelectedToHit(true); }
		}
	}
}


void UPlayerPawnComponent::SelectNextAttackTarget()
{
	UE_LOG(LogTemp, Log, TEXT("-_- not implementation"));
}
void UPlayerPawnComponent::SelectLastAttackTarget()
{
	if (LastAttackTarget)
	{
		if (CurrentAttackTarget)
		{
			IHitAbleInterface* CurrentHitableAttackActor = Cast<IHitAbleInterface>(CurrentAttackTarget);
			IHitAbleInterface* LastHitableAttackActor = Cast<IHitAbleInterface>(LastAttackTarget);
			if (CurrentHitableAttackActor && LastHitableAttackActor)
			{
				LastHitableAttackActor->SetIsSelectedToHit(true);
				CurrentHitableAttackActor->SetIsSelectedToHit(false);
			}
			AActor* Temp;
			Temp = CurrentAttackTarget;
			CurrentAttackTarget = LastAttackTarget;
			LastAttackTarget = Temp;
		}
		else
		{
			CurrentAttackTarget = LastAttackTarget;
			IHitAbleInterface* CurrentHitableAttackActor = Cast<IHitAbleInterface>(CurrentAttackTarget);
			if (CurrentHitableAttackActor) { CurrentHitableAttackActor->SetIsSelectedToHit(true); }

			CurrentAttackTarget = LastAttackTarget;
			LastAttackTarget = nullptr;
		}
	}
}

AActor* UPlayerPawnComponent::TryToGetHitAbleActor() const
{

	APlayerController* OwnerPC = OwnerPawn ? Cast<APlayerController>(OwnerPawn->GetController()) : nullptr;
	if (OwnerPC)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		OwnerPawn->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
		FVector EndTrace = CameraLocation + (CameraRotation.Vector() * OwnerPawn->GetMaxHyperopiaDistance());

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(OwnerPawn);
		FHitResult Hit;
		
		UKismetSystemLibrary::DrawDebugPoint(OwnerPawn, EndTrace, 10.f, FLinearColor::Blue, 0.1f);
		UKismetSystemLibrary::DrawDebugLine(OwnerPawn, OwnerPawn->GetActorLocation(), EndTrace, FLinearColor::Yellow, 0.1f, 5.f);

		if (GetWorld()->LineTraceSingleByChannel(Hit, OwnerPawn->GetActorLocation(), EndTrace, ECC_Visibility, TraceParams))
		{
			if (Cast<IHitAbleInterface>(Hit.GetActor()) && Hit.GetActor() != OwnerPawn)	
			{
				return Hit.GetActor();
			}
		}
	}

	return nullptr;
}

void UPlayerPawnComponent::Turn(float AxisValue)
{
	if (OwnerPawn)
	{
		OwnerPawn->AddControllerYawInput(AxisValue * BaseTurnRate);
	}
}
void UPlayerPawnComponent::LookUp(float AxisValue)
{
	if (OwnerPawn)
	{
		float CurrentPitch = OwnerPawn->GetControlRotation().Pitch;

		if (bIsConvertYAxis) { AxisValue = -AxisValue; }

		// 315 - 45
		if ((AxisValue > 0.f && (CurrentPitch < 50.f || CurrentPitch > 315.f)) || (AxisValue < 0.f && (CurrentPitch < 45.f || CurrentPitch > 310.f)))
		{
			OwnerPawn->AddControllerPitchInput(AxisValue * BaseTurnRate);
		}
	}
}

void UPlayerPawnComponent::OnPressNumberKeyboard(int32 NumberIndex)
{
	bIsWantedTogglePawnState ? TogglePawnState(NumberIndex) : UseProp(NumberIndex);
}

void UPlayerPawnComponent::UseProp(int32 NumberIndex)
{
	UE_LOG(LogTemp, Log, TEXT("-_- Use Prop Of Index: %d"), NumberIndex)
}
void UPlayerPawnComponent::TogglePawnState(int32 NumberIndex)
{
	if (OwnerPawn) { OwnerPawn->ServerToggleToNewPawnFormWithIndex(NumberIndex); }
}