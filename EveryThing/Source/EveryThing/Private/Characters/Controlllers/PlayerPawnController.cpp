// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawnController.h"

#include "GameFramework/InputSettings.h"

#include "EveryThingTypes.h"
#include "EveryThingAssetManager.h"
#include "Characters/GamePawn.h"
#include "Characters/PlayerPawnInterface.h"
#include "Characters/PlayerPawnComponent.h"
#include "Characters/Moves/Moves.h"
#include "Characters/Moves/AttackComponent.h"
#include "Characters/Moves/SkillComponent.h"
#include "SceneObject/HitAbleInterface.h"


void APlayerPawnController::SetupInputComponent()
{
	if (!InputComponent)
	{
		InputComponent = NewObject<UInputComponent>(this, UInputComponent::StaticClass());
		InputComponent->RegisterComponent();
	}

	// Clear all bindings;
	InputComponent->ClearActionBindings();
	InputComponent->AxisBindings.Empty();
	ResetAxisAndActionMapping();

	InputComponent->BindAxis("Turn", this, &APlayerPawnController::Turn);
	InputComponent->BindAxis("LookUp", this, &APlayerPawnController::LookUp);

	InputComponent->BindAction("SelectNextAttackActor", IE_Pressed, this, &APlayerPawnController::SelectNextAttackTarget);
	InputComponent->BindAction("SelectLastAttackActor", IE_Pressed, this, &APlayerPawnController::SelectLastAttackTarget);

	InputComponent->BindAction("TogglePawnState", IE_Pressed, this, &APlayerPawnController::StartTogglePawnState);
	InputComponent->BindAction("TogglePawnState", IE_Released, this, &APlayerPawnController::StopTogglePawnState);

	InputComponent->BindAction("TogglePawnSkin", IE_Pressed, this, &APlayerPawnController::StartTogglePawnSkin);
	InputComponent->BindAction("TogglePawnSkin", IE_Pressed, this, &APlayerPawnController::StartTogglePawnSkin);

	InputComponent->BindAction("NumberOne", IE_Pressed, this, &APlayerPawnController::NumberOne);
	InputComponent->BindAction("NumberTwo", IE_Pressed, this, &APlayerPawnController::NumberTwo);
	InputComponent->BindAction("NumberThree", IE_Pressed, this, &APlayerPawnController::NumberThree);
	InputComponent->BindAction("NumberFour", IE_Pressed, this, &APlayerPawnController::NumberFour);

	if (OwnerGamePawn)
	{
		ToggleToNewAttackComponent(OwnerGamePawn->GetAttackComponent());
		ToggleToNewSkillComponent(OwnerGamePawn->GetSkillComponent());
	}
}

void APlayerPawnController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	UE_LOG(LogTemp, Log, TEXT("-_- this is set pawn"));

	OwnerGamePawn = Cast<AGamePawn>(InPawn);
	IPlayerPawnInterface* OwnerPlayerPawn = Cast<IPlayerPawnInterface>(InPawn);
	OwnerPlayerPawnComp = OwnerPlayerPawn ? OwnerPlayerPawn->GetPlayerPawnComponent() : nullptr;

	if (InputComponent)
	{
		ToggleToNewAttackComponent(OwnerGamePawn ? OwnerGamePawn->GetAttackComponent() : nullptr);
		ToggleToNewSkillComponent(OwnerGamePawn ? OwnerGamePawn->GetSkillComponent() : nullptr);
	}
}


void APlayerPawnController::ResetAxisAndActionMapping()
{
	UInputSettings* InpueSettings = UInputSettings::GetInputSettings();
	if (!InpueSettings) { return; }

	UDataTable* InputActionDatable = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("InputAction"));
	if (InputActionDatable)
	{
		TArray<FInputAction*> ActionInfoInDatatable;
		InputActionDatable->GetAllRows<FInputAction>(TEXT("found all input axis mapping"), ActionInfoInDatatable);


		for (FInputAction* ActionInfo : ActionInfoInDatatable)
		{
			TArray<FInputActionKeyMapping> AllActionKeyMappings;
			InpueSettings->GetActionMappingByName(ActionInfo->ActionName, AllActionKeyMappings);
			for (const FInputActionKeyMapping& ActionKeyMapping : AllActionKeyMappings)
			{
				InpueSettings->RemoveActionMapping(ActionKeyMapping);
			}
		}

		for (FInputAction* ActionInfo : ActionInfoInDatatable)
		{
			InpueSettings->AddActionMapping(FInputActionKeyMapping(ActionInfo->ActionName, FKey(ActionInfo->Input), ActionInfo->bInShift, ActionInfo->bInCtrl, ActionInfo->bInAlt, ActionInfo->bInCmd));
		}
	}

	UDataTable* InputAxisDatable = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("InputAxis"));
	if (InputAxisDatable)
	{
		TArray<FInputAxis*> AxisInfoInDatatable;
		InputAxisDatable->GetAllRows<FInputAxis>(TEXT("found all input axis mapping"), AxisInfoInDatatable);


		for (FInputAxis* AxisInfo : AxisInfoInDatatable)
		{
			TArray<FInputAxisKeyMapping> AllAxisKeyMappings;
			InpueSettings->GetAxisMappingByName(AxisInfo->AxisName, AllAxisKeyMappings);
			for (const FInputAxisKeyMapping& AxisKeyMapping : AllAxisKeyMappings)
			{
				InpueSettings->RemoveAxisMapping(AxisKeyMapping);
			}
		}

		for (FInputAxis* AxisInfo : AxisInfoInDatatable)
		{
			InpueSettings->AddAxisMapping(FInputAxisKeyMapping(AxisInfo->AxisName, FKey(AxisInfo->Input), AxisInfo->AxisVlaue));
		}
	}
}

void APlayerPawnController::RemoveActionAndAxisBindings(const TArray<FName>& BindingsName)
{
	if (!InputComponent) { return; }

	for (int32 i = 0, l = InputComponent->AxisBindings.Num(); i < l; ++i)
	{
		if (BindingsName.Contains(InputComponent->AxisBindings[i].AxisName))
		{
			InputComponent->AxisBindings.RemoveAt(i);
			--i; --l;
		}
	}

	for (int32 i = 0, l = InputComponent->GetNumActionBindings(); i < l; ++i)
	{
		if (BindingsName.Contains(InputComponent->GetActionBinding(i).ActionName))
		{
			InputComponent->RemoveActionBinding(i);
			--i; --l;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/// Game match
void APlayerPawnController::ClientSetSpectatorCamera_Implementation(FVector CameraLocation, FRotator CameraRotation) {}
void APlayerPawnController::ClientGameStarted_Implementation() {}
void APlayerPawnController::ClientStartOnlineGame_Implementation() {}
void APlayerPawnController::ClientEndOnlineGame_Implementation() {}

void APlayerPawnController::ClientSendRoundEndEvent_Implementation(bool bIsWinner, int32 ExpendedTimeInSeconds) {}


//////////////////////////////////////////////////////////////////////////
/// For IGamePawnControllerInterface
AActor* APlayerPawnController::GetAttackTarget() { return OwnerPlayerPawnComp ? OwnerPlayerPawnComp->GetAttackTarget() : nullptr; }

//////////////////////////////////////////////////////////////////////////
/// Visual Angle
void APlayerPawnController::Turn(float AxisValue) { if (AxisValue != 0.f && OwnerPlayerPawnComp) { OwnerPlayerPawnComp->Turn(AxisValue); } }
void APlayerPawnController::LookUp(float AxisValue) { if (AxisValue != 0.f && OwnerPlayerPawnComp) { OwnerPlayerPawnComp->LookUp(AxisValue); } }

//////////////////////////////////////////////////////////////////////////
/// Game Pawn Form And Prop Use
void APlayerPawnController::StartTogglePawnState() { if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->SetIsWantedTogglePawnForm(true); } }
void APlayerPawnController::StopTogglePawnState() { if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->SetIsWantedTogglePawnForm(false); } }

void APlayerPawnController::StartTogglePawnSkin() { if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->SetIsWantedTogglePawnSkin(true); } }
void APlayerPawnController::StopTogglePawnSkin() { if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->SetIsWantedTogglePawnSkin(false); } }


void APlayerPawnController::NumberOne() { if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->OnPressNumberKeyboard(0); } }
void APlayerPawnController::NumberTwo() { if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->OnPressNumberKeyboard(1); } }
void APlayerPawnController::NumberThree() { if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->OnPressNumberKeyboard(2); } }
void APlayerPawnController::NumberFour() {if (OwnerPlayerPawnComp) {OwnerPlayerPawnComp->OnPressNumberKeyboard(3);}}

void APlayerPawnController::SelectNextAttackTarget() { if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->SelectNextAttackTarget(); } }
void APlayerPawnController::SelectLastAttackTarget() { if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->SelectLastAttackTarget(); } }

//////////////////////////////////////////////////////////////////////////
/// _Skilledness

void APlayerPawnController::StartSkilledness(FMoves* Skillness) { if (Skillness->SkillednessComp && Skillness->StartSkilledness) { (Skillness->SkillednessComp->*(Skillness->StartSkilledness))(); } }
void APlayerPawnController::StopSkilledness(FMoves* Skillness) { if (Skillness->SkillednessComp && Skillness->StopSkilledness) { (Skillness->SkillednessComp->*(Skillness->StopSkilledness))(); } }
void APlayerPawnController::ExcuteSkilledness(FMoves* Skillness, float Value) { if (Skillness->SkillednessComp && Skillness->ExcuteSkilledness) { (Skillness->SkillednessComp->*(Skillness->ExcuteSkilledness))(Value); } }

void APlayerPawnController::BindSkillednessInputEvent(FMoves* Skillness, void(APlayerPawnController::*InStartKilledness)(), void(APlayerPawnController::*InStopKilledness)(), void(APlayerPawnController::*InExcuteKilledness)(float))
{
	if (Skillness && InputComponent)
	{
		if (Skillness->bIsEnableActionPressed) { InputComponent->BindAction(Skillness->BindingName, IE_Pressed, this, InStartKilledness); }
		if (Skillness->bIsEnableActionReleased) { InputComponent->BindAction(Skillness->BindingName, IE_Released, this, InStopKilledness); }
		if (Skillness->bIsEnableAxis) { InputComponent->BindAxis(Skillness->BindingName, this, InExcuteKilledness); }
	}
}

//////////////////////////////////////////////////////////////////////////
/// Attack

void APlayerPawnController::ToggleToNewAttackComponent(UAttackComponent* InAttackComp)
{
	if (CurrentAttackComponent) { RemoveActionAndAxisBindings(TArray<FName>{"CommonAttack", "SpecialAttack"}); }

	CurrentAttackComponent = InAttackComp;

	if (CurrentAttackComponent)
	{
		CommonAttack = CurrentAttackComponent->GetCommantAttackSkilledness();
		BindSkillednessInputEvent(CommonAttack, &APlayerPawnController::StartCommonAttack, &APlayerPawnController::StopCommonAttack, &APlayerPawnController::ExcuteCommonAttack);
		SpecialAttack = CurrentAttackComponent->GetSpecialAttackSkilledness();
		BindSkillednessInputEvent(SpecialAttack, &APlayerPawnController::StartSpecialAttack, &APlayerPawnController::StopSpecialAttack, &APlayerPawnController::ExcuteSpecialAttack);
	}
}

void APlayerPawnController::StartCommonAttack() { if (CommonAttack) { StartSkilledness(CommonAttack); } }
void APlayerPawnController::StopCommonAttack() { if (CommonAttack) { StopSkilledness(CommonAttack); } }
void APlayerPawnController::ExcuteCommonAttack(float AxisValue) { if (AxisValue != 0.f && CommonAttack) { ExcuteSkilledness(CommonAttack, AxisValue); } }

void APlayerPawnController::StartSpecialAttack() { if (SpecialAttack) { StartSkilledness(SpecialAttack); } }
void APlayerPawnController::StopSpecialAttack() { if (SpecialAttack) { StopSkilledness(SpecialAttack); } }
void APlayerPawnController::ExcuteSpecialAttack(float AxisValue) { if (AxisValue != 0.f && SpecialAttack) { ExcuteSkilledness(SpecialAttack, AxisValue); } }

//////////////////////////////////////////////////////////////////////////
/// Skill
void APlayerPawnController::ToggleToNewSkillComponent(USkillComponent* InSkillComp)
{
	if (CurrentSkillComponent) { RemoveActionAndAxisBindings(TArray<FName>{"FirstSkill", "SecondSkill", "UltimateSkill"}); }

	CurrentSkillComponent = InSkillComp;

	if (CurrentSkillComponent)
	{
		FirstSkill = CurrentSkillComponent->GetFirstSkillSkilledness();
		BindSkillednessInputEvent(FirstSkill, &APlayerPawnController::StartFirstSkill, &APlayerPawnController::StopFirstSkill, &APlayerPawnController::ExcuteFirstSkill);

		SecondSkill = CurrentSkillComponent->GetSecondSkillSkilledness();
		BindSkillednessInputEvent(SecondSkill, &APlayerPawnController::StartSecondSkill, &APlayerPawnController::StopSecondSkill, &APlayerPawnController::ExcuteSecondSkill);

		UltimateSkill = CurrentSkillComponent->GetUltimateSkillSkilledness();
		BindSkillednessInputEvent(UltimateSkill, &APlayerPawnController::StartUltimateSkill, &APlayerPawnController::StopUltimateSkill, &APlayerPawnController::ExcuteUltimateSkill);
	}
}

void APlayerPawnController::StartFirstSkill() { if (FirstSkill) { StartSkilledness(FirstSkill); } }
void APlayerPawnController::StopFirstSkill() { if (FirstSkill) { StopSkilledness(FirstSkill); } }
void APlayerPawnController::ExcuteFirstSkill(float AxisValue) { if (AxisValue != 0.f && FirstSkill) { ExcuteSkilledness(FirstSkill, AxisValue); } }

void APlayerPawnController::StartSecondSkill() { if (SecondSkill) { StartSkilledness(SecondSkill); } }
void APlayerPawnController::StopSecondSkill() { if (SecondSkill) { StopSkilledness(SecondSkill); } }
void APlayerPawnController::ExcuteSecondSkill(float AxisValue) { if (AxisValue != 0.f && SecondSkill) { ExcuteSkilledness(SecondSkill, AxisValue); } }

void APlayerPawnController::StartUltimateSkill() { if (UltimateSkill) { StartSkilledness(UltimateSkill); } }
void APlayerPawnController::StopUltimateSkill() { if (UltimateSkill) { StopSkilledness(UltimateSkill); } }
void APlayerPawnController::ExcuteUltimateSkill(float AxisValue) { if (AxisValue != 0.f && UltimateSkill) { ExcuteSkilledness(UltimateSkill, AxisValue); } }