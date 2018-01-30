// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawnController.h"

#include "GameFramework/InputSettings.h"

#include "EveryThingTypes.h"
#include "EveryThingGameInstance.h"
#include "UI/Game/EveryThingGameHUD.h"
#include "EveryThingAssetManager.h"
#include "Characters/GamePawn.h"
#include "Characters/PlayerPawnInterface.h"
#include "Characters/PlayerPawnComponent.h"
#include "Characters/Moves/Moves.h"
#include "Characters/Moves/AttackComponent.h"
#include "Characters/Moves/SkillComponent.h"
#include "Characters/Movement/Components/GamePawnMovementComponent.h"
#include "SceneObject/HitAbleInterface.h"


void APlayerPawnController::BeginPlay()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void APlayerPawnController::SetupInputComponent()
{
	if (!InputComponent)
	{
		InputComponent = NewObject<UInputComponent>(this, UInputComponent::StaticClass());
		InputComponent->RegisterComponent();
	}
}

void APlayerPawnController::RebindInput()
{
	if (!InputComponent) { return; }

	// Clear all bindings;
	InputComponent->ClearActionBindings();
	InputComponent->AxisBindings.Empty();
	ResetAxisAndActionMapping();

	InputComponent->BindAxis("Turn", this, &APlayerPawnController::Turn);
	InputComponent->BindAxis("LookUp", this, &APlayerPawnController::LookUp);

	InputComponent->BindAction("SelectNextAttackActor", IE_Pressed, this, &APlayerPawnController::SelectNextAttackTarget);
	InputComponent->BindAction("SelectLastAttackActor", IE_Pressed, this, &APlayerPawnController::SelectLastAttackTarget);


	InputComponent->BindAction("TogglePawn", IE_Pressed, this, &APlayerPawnController::StartTogglePawn);
	InputComponent->BindAction("TogglePawn", IE_Released, this, &APlayerPawnController::StopTogglePawn);

	InputComponent->BindAction("TogglePawnState", IE_Pressed, this, &APlayerPawnController::StartTogglePawnState);
	InputComponent->BindAction("TogglePawnState", IE_Released, this, &APlayerPawnController::StopTogglePawnState);

	InputComponent->BindAction("TogglePawnSkin", IE_Pressed, this, &APlayerPawnController::StartTogglePawnSkin);
	InputComponent->BindAction("TogglePawnSkin", IE_Released, this, &APlayerPawnController::StopTogglePawnSkin);

	InputComponent->BindAction("ToggleGameMenu", IE_Pressed, this, &APlayerPawnController::ToggleGameMenu);


	InputComponent->BindAction("NumberOne", IE_Pressed, this, &APlayerPawnController::NumberOne);
	InputComponent->BindAction("NumberTwo", IE_Pressed, this, &APlayerPawnController::NumberTwo);
	InputComponent->BindAction("NumberThree", IE_Pressed, this, &APlayerPawnController::NumberThree);
	InputComponent->BindAction("NumberFour", IE_Pressed, this, &APlayerPawnController::NumberFour);

	if (OwnerGamePawn)
	{
		ToggleToNewAttackComponent(OwnerGamePawn->GetAttackComponent());
		ToggleToNewSkillComponent(OwnerGamePawn->GetSkillComponent());
		
		UGamePawnMovementComponent* OwnerPlayerMovementComp = OwnerGamePawn->GetGamePawnMovementComponent();
		if (OwnerPlayerMovementComp) { OwnerPlayerMovementComp->RebindInputComp(InputComponent); }
	}
}

void APlayerPawnController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	UE_LOG(LogTemp, Log, TEXT("-_- this is set pawn"));

	OwnerGamePawn = Cast<AGamePawn>(InPawn);
	IPlayerPawnInterface* OwnerPlayerPawn = Cast<IPlayerPawnInterface>(InPawn);
	OwnerPlayerPawnComp = OwnerPlayerPawn ? OwnerPlayerPawn->GetPlayerPawnComponent() : nullptr;

	RebindInput();
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
/// For IGamePawnControllerInterface
AActor* APlayerPawnController::GetAttackTarget() { return OwnerPlayerPawnComp ? OwnerPlayerPawnComp->GetAttackTarget() : nullptr; }

//////////////////////////////////////////////////////////////////////////
/// Visual Angle
void APlayerPawnController::Turn(float AxisValue) { if (AxisValue != 0.f && OwnerPlayerPawnComp) { OwnerPlayerPawnComp->Turn(AxisValue); } }
void APlayerPawnController::LookUp(float AxisValue) { if (AxisValue != 0.f && OwnerPlayerPawnComp) { OwnerPlayerPawnComp->LookUp(AxisValue); } }


//////////////////////////////////////////////////////////////////////////
void APlayerPawnController::ToggleGameMenu()
{
	UE_LOG(LogTemp, Log, TEXT("-_- toggle to game menu"));

	AEveryThingGameHUD* OwnerETGH = Cast<AEveryThingGameHUD>(GetHUD());
	if (OwnerETGH) { OwnerETGH->ToggleGameMenu(); }
}

//////////////////////////////////////////////////////////////////////////
/// About Toggle all and prop

void APlayerPawnController::StartTogglePawn() { if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->SetIsWantedTogglePawn(true); } }
void APlayerPawnController::StopTogglePawn() { if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->SetIsWantedTogglePawn(false); } }


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
/// Attack

void APlayerPawnController::ToggleToNewAttackComponent(UAttackComponent* InAttackComp)
{
	if (CurrentAttackComponent) { RemoveActionAndAxisBindings(TArray<FName>{"CommonAttack", "SpecialAttack"}); }

	CurrentAttackComponent = InAttackComp;

	if (CurrentAttackComponent)
	{
		FMoves* CommonAttack = CurrentAttackComponent->GetCommantAttackSkilledness();
		if (CommonAttack) { CommonAttack->RebindInput(InputComponent); }
		FMoves* SpecialAttack = CurrentAttackComponent->GetSpecialAttackSkilledness();
		if (SpecialAttack) { SpecialAttack->RebindInput(InputComponent); }
	}
}

//////////////////////////////////////////////////////////////////////////
/// Skill
void APlayerPawnController::ToggleToNewSkillComponent(USkillComponent* InSkillComp)
{
	if (CurrentSkillComponent) { RemoveActionAndAxisBindings(TArray<FName>{"FirstSkill", "SecondSkill", "UltimateSkill"}); }

	CurrentSkillComponent = InSkillComp;

	if (CurrentSkillComponent)
	{
		FMoves* FirstSkill = CurrentSkillComponent->GetFirstSkillSkilledness();
		if (FirstSkill) { FirstSkill->RebindInput(InputComponent); }

		FMoves* SecondSkill = CurrentSkillComponent->GetSecondSkillSkilledness();
		if (SecondSkill) { SecondSkill->RebindInput(InputComponent); }

		FMoves* UltimateSkill = CurrentSkillComponent->GetUltimateSkillSkilledness();
		if (UltimateSkill) { UltimateSkill->RebindInput(InputComponent); }
	}
}