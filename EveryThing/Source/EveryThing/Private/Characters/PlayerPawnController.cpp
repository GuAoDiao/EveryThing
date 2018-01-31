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

	InputComponent->BindAction("ToggleGameMenu", IE_Pressed, this, &APlayerPawnController::ToggleGameMenu);

	InputComponent->BindAxis("Turn", this, &APlayerPawnController::Turn);
	InputComponent->BindAxis("LookUp", this, &APlayerPawnController::LookUp);



	if (OwnerGamePawn)
	{
		ToggleToNewAttackComponent(OwnerGamePawn->GetAttackComponent());
		ToggleToNewSkillComponent(OwnerGamePawn->GetSkillComponent());
		
		UGamePawnMovementComponent* OwnerPlayerMovementComp = OwnerGamePawn->GetGamePawnMovementComponent();
		if (OwnerPlayerMovementComp) { OwnerPlayerMovementComp->RebindInputComp(InputComponent); }
	}
	if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->RebindInputComp(InputComponent); }
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
/// Attack

void APlayerPawnController::ToggleToNewAttackComponent(UAttackComponent* InAttackComp)
{
	RemoveActionAndAxisBindings(TArray<FName>{"CommonAttack", "SpecialAttack"});

	if (InAttackComp)
	{
		FMoves* CommonAttack = InAttackComp->GetCommantAttackSkilledness();
		if (CommonAttack) { CommonAttack->RebindInput(InputComponent); }
		FMoves* SpecialAttack = InAttackComp->GetSpecialAttackSkilledness();
		if (SpecialAttack) { SpecialAttack->RebindInput(InputComponent); }
	}
}

//////////////////////////////////////////////////////////////////////////
/// Skill
void APlayerPawnController::ToggleToNewSkillComponent(USkillComponent* InSkillComp)
{
	RemoveActionAndAxisBindings(TArray<FName>{"FirstSkill", "SecondSkill", "UltimateSkill"});

	if (InSkillComp)
	{
		FMoves* FirstSkill = InSkillComp->GetFirstSkillSkilledness();
		if (FirstSkill) { FirstSkill->RebindInput(InputComponent); }

		FMoves* SecondSkill = InSkillComp->GetSecondSkillSkilledness();
		if (SecondSkill) { SecondSkill->RebindInput(InputComponent); }

		FMoves* UltimateSkill = InSkillComp->GetUltimateSkillSkilledness();
		if (UltimateSkill) { UltimateSkill->RebindInput(InputComponent); }
	}
}