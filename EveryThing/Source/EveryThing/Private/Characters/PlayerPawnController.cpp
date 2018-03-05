// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawnController.h"

#include "UnrealNetwork.h"
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
#include "Online/EveryThingPlayerState.h"


#define LOCTEXT_NAMESPACE "Everything_Online_PlayerPawnController"

void APlayerPawnController::BeginPlay()
{
	Super::BeginPlay();


	AEveryThingPlayerState* OwnerETPS = Cast<AEveryThingPlayerState>(PlayerState);
	UClass* CurrentPawnClass = GetPawn() ? GetPawn()->GetClass() : nullptr;
	if (OwnerETPS &&CurrentPawnClass)
	{
		FPlayerInfo PlayerInfo= OwnerETPS->GetPlayerInfo();

		CurrentRoleName = NAME_None;
		
		for (const FName RoleName : PlayerInfo.AllHaveRolesName)
		{
			TSoftClassPtr<AGamePawn> TargetPawnSoftClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetRoleClassFromName(RoleName);
			UClass* TargetPawnClass = TargetPawnSoftClass ? TargetPawnSoftClass->GetClass() : nullptr;
			if (!TargetPawnClass) { continue; }

			if (CurrentPawnClass->IsChildOf(TargetPawnClass) || TargetPawnClass->IsChildOf(CurrentPawnClass))
			{
				CurrentRoleName = RoleName;
				break;
			}
		}
	}


	SetInputMode(FInputModeGameOnly());
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

	InputComponent->BindAction("ToggleGameMenu", IE_Pressed, this, &APlayerPawnController::DisplayGameMenu);

	InputComponent->BindAxis("Turn", this, &APlayerPawnController::Turn);
	InputComponent->BindAxis("LookUp", this, &APlayerPawnController::LookUp);

	InputComponent->BindAction("TogglePawn", IE_Pressed, this, &APlayerPawnController::StartToggleRole);
	InputComponent->BindAction("TogglePawn", IE_Released, this, &APlayerPawnController::StopToggleRole);

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
/// UI
void APlayerPawnController::DisplayGameMenu()
{
	UE_LOG(LogTemp, Log, TEXT("-_- toggle to game menu"));

	AEveryThingGameHUD* OwnerETGH = Cast<AEveryThingGameHUD>(GetHUD());
	if (OwnerETGH) { OwnerETGH->DisplayGameMenu(); }
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



//////////////////////////////////////////////////////////////////////////
/// Toggle Role
void APlayerPawnController::StartToggleRole()
{
	AEveryThingGameHUD* OwnerETGH = Cast<AEveryThingGameHUD>(GetHUD());
	if (OwnerETGH) { OwnerETGH->ToggleSelectRolesBox(true); }

	if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->GetOnPressNumberKeyboardDelegate().BindUObject(this, &APlayerPawnController::ToggoleRole); }

	bIsWantedTogglePawn = true;
}
void APlayerPawnController::StopToggleRole()
{
	AEveryThingGameHUD* OwnerETGH = Cast<AEveryThingGameHUD>(GetHUD());
	if (OwnerETGH) { OwnerETGH->ToggleSelectRolesBox(false); }

	if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->GetOnPressNumberKeyboardDelegate().Unbind(); }

	bIsWantedTogglePawn = false;
}

void APlayerPawnController::ToggoleRole(int32 NumberIndex)
{
	// Get owning EverythingPlayerState and the world is exists
	AEveryThingPlayerState* OwnerETPS = Cast<AEveryThingPlayerState>(PlayerState);
	if (!GetWorld() ||!OwnerETPS)
	{
		OnToggleToTargetRoleFailureDelegate.Broadcast(FName("None"), LOCTEXT("", "Can't find Game World Or Owner Player State."));
		return;
	}
	
	// try to find Target Role Name is exists
	const FPlayerInfo& PlayerInfo = OwnerETPS->GetPlayerInfo();
	if (!PlayerInfo.AllHaveRolesName.IsValidIndex(NumberIndex))
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("Index"), NumberIndex);
		OnToggleToTargetRoleFailureDelegate.Broadcast(FName("None"), FText::Format(LOCTEXT("", "Can'f find Target role name with index : {Index}."), Arguments));
		return;
	}
	const FName& TargetRoleName = PlayerInfo.AllHaveRolesName[NumberIndex];

	// Get Current and Target Pawn class 
	UEveryThingAssetManager* AssetManager = UEveryThingAssetManager::GetAssetManagerInstance();
	TSoftClassPtr<AGamePawn> CurrentPawnSoftClass = AssetManager->GetRoleClassFromName(CurrentRoleName);
	TSoftClassPtr<AGamePawn> TargetPawnSoftClass = AssetManager->GetRoleClassFromName(TargetRoleName);

	UClass* CurrentPawnClass = CurrentPawnSoftClass ? CurrentPawnSoftClass->GetClass() : nullptr;
	UClass* TargetPawnClass = TargetPawnSoftClass ? TargetPawnSoftClass->GetClass() : nullptr;

	// Check target pawn class is exists
	if (!TargetPawnClass)
	{
		OnToggleToTargetRoleFailureDelegate.Broadcast(TargetRoleName, LOCTEXT("", "Can't find Target Role Class."));
		return;
	}

	// check target pawn and current pawn isn't Parent-child
	if (CurrentPawnClass->IsChildOf(TargetPawnClass) || TargetPawnClass->IsChildOf(CurrentPawnClass))
	{
		OnToggleToTargetRoleFailureDelegate.Broadcast(TargetRoleName, LOCTEXT("", "Already is the target Role, needn't to toggle."));
		return;
	}

	// after code only run on server
	if (!HasAuthority())
	{
		ServerToggleRole(NumberIndex);
		return;
	}


	// Hide current Pawn and disable it's collision
	APawn* OwnerPawn = GetPawn();
	
	check(OwnerPawn);

	OwnerPawn->SetActorEnableCollision(false);
	OwnerPawn->SetActorHiddenInGame(true);

	// spawn and possess new pawn
	FVector Location = OwnerPawn->GetActorLocation();
	Location.Z += 100.f;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AGamePawn* NewPawn = GetWorld()->SpawnActor<AGamePawn>(TargetPawnClass, Location, OwnerPawn->GetActorForwardVector().Rotation());
	Possess(NewPawn);


	// destroy old pawn
	OwnerPawn->Destroy();

	// update current pawn class
	OnToggleToTargetRoleSuccessDelegate.Broadcast(TargetRoleName);
	
	StopToggleRole();

	CurrentRoleName = TargetRoleName;
}

bool APlayerPawnController::ServerToggleRole_Validate(int32 NumberIndex) { return true; }
void APlayerPawnController::ServerToggleRole_Implementation(int32 NumberIndex)
{
	ToggoleRole(NumberIndex);
}




void APlayerPawnController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerPawnController, CurrentRoleName);
}



#undef LOCTEXT_NAMESPACE