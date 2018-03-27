// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawnComponent.h"

#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Characters/GamePawn.h"
#include "Online/PlayerController_Game.h"
#include "Online/EveryThingPlayerState_Game.h"
#include "UI/EveryThingHUD_Game.h"

UPlayerPawnComponent::UPlayerPawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	OwnerPawn = Cast<AGamePawn>(GetOwner());
	
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

	/// Attack Target
	CurrentAttackTarget = nullptr;
	LastAttackTarget = nullptr;
}


void UPlayerPawnComponent::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlayerPawnComponent::DelayBindEveryThingPlayerState, 1.f, false);
}


void UPlayerPawnComponent::DelayBindEveryThingPlayerState()
{
	APlayerController* OwnerPC = OwnerPawn ? Cast<APlayerController>(OwnerPawn->GetController()) : nullptr;
	AEveryThingPlayerState_Game* OwnerETPS = OwnerPC ? Cast<AEveryThingPlayerState_Game>(OwnerPC->PlayerState) : nullptr;
	if (OwnerETPS)
	{
		OnUpdatePlayerInfo(OwnerETPS->GetPlayerInfo());
		OwnerETPS->OnUpdatePlayerInfoDelegate.AddUObject(this, &UPlayerPawnComponent::OnUpdatePlayerInfo);
	}
}

void UPlayerPawnComponent::OnUpdatePlayerInfo(const FPlayerInfo& InPlayerInfo)
{
	if (OwnerPawn)
	{
		OwnerPawn->AllHaveRoleFormName.Empty();
		for (const FName& FormName : OwnerPawn->AllRoleFormName)
		{
			if (InPlayerInfo.AllHaveGamePawnFormNames.Contains(FormName))
			{
				OwnerPawn->AllHaveRoleFormName.AddUnique(FormName);
			}
		}

		OwnerPawn->AllHaveRoleSkinName.Empty();
		for (const FName& SkinName : OwnerPawn->AllRoleSkinName)
		{
			if (InPlayerInfo.AllHaveGamePawnSkinNames.Contains(SkinName))
			{
				OwnerPawn->AllHaveRoleSkinName.AddUnique(SkinName);
			}
		}
	}
}

void UPlayerPawnComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (OwnerPawn && OwnerPawn->GetController() && OwnerPawn->GetController()->IsLocalController())
	{
		AActor* SelectedHitAbleActor = TryToGetHitAbleActor();

		if (SelectedHitAbleActor && SelectedHitAbleActor != CurrentAttackTarget && SelectedHitAbleActor!= OwnerPawn)
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
//////////////////////////////////////////////////////////////////////////
/// Input
void UPlayerPawnComponent::RebindInputComp(class UInputComponent* OwnerInputComp)
{
	if (!OwnerInputComp) { return; }

	OwnerInputComp->BindAction("SelectNextAttackActor", IE_Pressed, this, &UPlayerPawnComponent::SelectNextAttackTarget);
	OwnerInputComp->BindAction("SelectLastAttackActor", IE_Pressed, this, &UPlayerPawnComponent::SelectLastAttackTarget);

	OwnerInputComp->BindAction("TogglePawnState", IE_Pressed, this, &UPlayerPawnComponent::StartTogglePawnForm);
	OwnerInputComp->BindAction("TogglePawnState", IE_Released, this, &UPlayerPawnComponent::StopTogglePawnForm);

	OwnerInputComp->BindAction("TogglePawnSkin", IE_Pressed, this, &UPlayerPawnComponent::StartTogglePawnSkin);
	OwnerInputComp->BindAction("TogglePawnSkin", IE_Released, this, &UPlayerPawnComponent::StopTogglePawnSkin);


	FInputActionBinding Number("Number", IE_Pressed);
	Number.ActionDelegate.GetDelegateWithKeyForManualSet().BindLambda( [this](FKey InKey)
	{
		if (InKey == EKeys::One) { OnPressNumberKeyboard(0); }
		else if (InKey == EKeys::Two) { OnPressNumberKeyboard(1); }
		else if (InKey == EKeys::Three) { OnPressNumberKeyboard(2); }
		else if (InKey == EKeys::Four) { OnPressNumberKeyboard(3); }
		else if (InKey == EKeys::Five) { OnPressNumberKeyboard(4); }
		else if (InKey == EKeys::Six) { OnPressNumberKeyboard(5); }
		else if (InKey == EKeys::Seven) { OnPressNumberKeyboard(6); }
		else if (InKey == EKeys::Eight) { OnPressNumberKeyboard(7); }
		else if (InKey == EKeys::Nine) { OnPressNumberKeyboard(8); }
		else if (InKey == EKeys::Zero) { OnPressNumberKeyboard(9); }
	});

	OwnerInputComp->AddActionBinding(Number);
}


//////////////////////////////////////////////////////////////////////////
/// Visual Angle
void UPlayerPawnComponent::Turn(float AxisValue)
{
	if (OwnerPawn) { OwnerPawn->AddControllerYawInput(AxisValue * BaseTurnRate); }
}
void UPlayerPawnComponent::LookUp(float AxisValue)
{
	if (OwnerPawn)
	{
		float CurrentPitch = OwnerPawn->GetControlRotation().Pitch;

		if (bIsConvertYAxis) { AxisValue = -AxisValue; }

		// 315 - 90
		if ((AxisValue > 0.f && (CurrentPitch < 95.f || CurrentPitch > 315.f)) || (AxisValue < 0.f && (CurrentPitch < 90.f || CurrentPitch > 310.f)))
		{
			OwnerPawn->AddControllerPitchInput(AxisValue * BaseTurnRate);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/// Select Attack Target
void UPlayerPawnComponent::SelectNextAttackTarget()
{
	UE_LOG(LogTemp, Log, TEXT("-_- not implementation"));
}
void UPlayerPawnComponent::SelectLastAttackTarget()
{
	if (LastAttackTarget)
	{
		IHitAbleInterface* HitableAttackActor;
		HitableAttackActor = Cast<IHitAbleInterface>(CurrentAttackTarget);
		if (HitableAttackActor) { HitableAttackActor->SetIsSelectedToHit(false); }

		HitableAttackActor = Cast<IHitAbleInterface>(LastAttackTarget);
		if (HitableAttackActor) { HitableAttackActor->SetIsSelectedToHit(true); }

		AActor* Temp;
		Temp = CurrentAttackTarget;
		CurrentAttackTarget = LastAttackTarget;
		LastAttackTarget = Temp;
	}
}

AActor* UPlayerPawnComponent::TryToGetHitAbleActor() const
{

	APlayerController* OwnerPC = OwnerPawn ? Cast<APlayerController>(OwnerPawn->GetController()) : nullptr;
	if (OwnerPC)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		OwnerPC->GetPlayerViewPoint(CameraLocation, CameraRotation);
		FVector EndTrace = CameraLocation + (CameraRotation.Vector() * OwnerPawn->GetMaxHyperopiaDistance());

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(OwnerPawn);
		FHitResult Hit;
		
		UKismetSystemLibrary::DrawDebugPoint(OwnerPawn, EndTrace, 10.f, FLinearColor::Blue, 0.1f);
		UKismetSystemLibrary::DrawDebugLine(OwnerPawn, OwnerPawn->GetActorLocation(), EndTrace, FLinearColor::Yellow, 0.1f, 5.f);

		if (GetWorld()->LineTraceSingleByChannel(Hit, OwnerPawn->GetActorLocation(), EndTrace, ECC_Visibility, TraceParams))
		{
			if (Cast<IHitAbleInterface>(Hit.GetActor()))	
			{
				return Hit.GetActor();
			}
		}
	}

	return nullptr;
}




//////////////////////////////////////////////////////////////////////////
/// Game Pawn Form And Prop Use
void UPlayerPawnComponent::OnPressNumberKeyboard(int32 NumberIndex)
{
	if (OnPressNumberKeyboardDelegate.IsBound())
	{
		OnPressNumberKeyboardDelegate.Execute(NumberIndex);
	}
	else
	{
		UseProp(NumberIndex);
	}
}

void UPlayerPawnComponent::UseProp(int32 NumberIndex)
{
	UE_LOG(LogTemp, Log, TEXT("-_- Use Prop Of Index: %d"), NumberIndex)
}

void UPlayerPawnComponent::ToggleRoleForm(int32 NumberIndex) { if (OwnerPawn) { OwnerPawn->ToggleToNewFormWithIndex(NumberIndex); } }
void UPlayerPawnComponent::ToggleRoleSkin(int32 NumberIndex) { if (OwnerPawn) { OwnerPawn->ToggleToNewSkinWithIndex(NumberIndex); } }


AEveryThingHUD_Game* UPlayerPawnComponent::GetEveryThingGameHUD() const
{
	APlayerController* OwnerPC = OwnerPawn ? Cast<APlayerController>(OwnerPawn->GetController()) : nullptr;
	return OwnerPC ? Cast<AEveryThingHUD_Game>(OwnerPC->GetHUD()) : nullptr;
}

void UPlayerPawnComponent::StartTogglePawnSkin()
{
	AEveryThingHUD_Game* OwnerETGH = GetEveryThingGameHUD();
	if (OwnerETGH) { OwnerETGH->ToggleSelectSkinsBox(true); }

	GetOnPressNumberKeyboardDelegate().BindUObject(this, &UPlayerPawnComponent::ToggleRoleSkin);
}

void UPlayerPawnComponent::StopTogglePawnSkin()
{
	AEveryThingHUD_Game* OwnerETGH = GetEveryThingGameHUD();
	if (OwnerETGH) { OwnerETGH->ToggleSelectSkinsBox(false); }

	GetOnPressNumberKeyboardDelegate().Unbind();
}


void UPlayerPawnComponent::StartTogglePawnForm()
{
	AEveryThingHUD_Game* OwnerETGH = GetEveryThingGameHUD();
	if (OwnerETGH) { OwnerETGH->ToggleSelectFormsBox(true); }

	GetOnPressNumberKeyboardDelegate().BindUObject(this, &UPlayerPawnComponent::ToggleRoleForm);
}

void UPlayerPawnComponent::StopTogglePawnForm()
{
	AEveryThingHUD_Game* OwnerETGH = GetEveryThingGameHUD();
	if (OwnerETGH) { OwnerETGH->ToggleSelectFormsBox(false); }

	GetOnPressNumberKeyboardDelegate().Unbind();
}