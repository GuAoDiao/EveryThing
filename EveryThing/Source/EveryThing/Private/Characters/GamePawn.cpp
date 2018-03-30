// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawn.h"

#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

#include "EveryThingAssetManager.h"
#include "Online/EveryThingGameMode_Game.h"
#include "Characters/Form/GamePawnForm.h"
#include "Characters/Skin/GamePawnSkin.h"
#include "Characters/Moves/AttackComponent.h"
#include "Characters/Moves/SkillComponent.h"
#include "Online/PlayerController_Game.h"
#include "Characters/Movement/Components/GamePawnMovementComponent.h"

#define LOCTEXT_NAMESPACE "Everything_Characters_GamePawn"

AGamePawn::AGamePawn()
{
	bReplicates = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	
	StaticMeshComp->SetSimulatePhysics(true);

	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComp->SetCollisionObjectType(ECC_Pawn);
	StaticMeshComp->SetCollisionResponseToAllChannels(ECR_Block);
	
	StaticMeshComp->BodyInstance.bUseCCD = true;

	StaticMeshComp->bGenerateOverlapEvents = true;
	StaticMeshComp->SetNotifyRigidBodyCollision(true);
	StaticMeshComp->OnComponentHit.AddDynamic(this, &AGamePawn::OnHit);

	SetRootComponent(StaticMeshComp);

	MovementComp = nullptr;
	
	OwnerAttackComp = nullptr;
	OwnerSkillComp = nullptr;

	PrimaryActorTick.bCanEverTick = true;
}

void AGamePawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateInfo();
}

void AGamePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	OnPossessedByControllerDelegate.Broadcast(NewController);
}


void AGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		ChangeStaminaTick(DeltaTime);
	}
}

void AGamePawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurrentRoleSkin) { delete CurrentRoleSkin; }
	if (CurrentRoleForm) { delete CurrentRoleForm; }

	Super::EndPlay(EndPlayReason);
}

void AGamePawn::SetIsSelectedToHit(bool bInIsSelectedToHit)
{
	if (bInIsSelectedToHit)
	{
		StaticMeshComp->SetRenderCustomDepth(true);
		StaticMeshComp->CustomDepthStencilValue = 2.f;
	}
	else
	{
		StaticMeshComp->SetRenderCustomDepth(false);
		StaticMeshComp->CustomDepthStencilValue = 0.f;
	}
}

void AGamePawn::AcceptHitFrom(AActor* OtherActor, FVector NormalInpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		AEveryThingGameMode_Game* GameMode = Cast<AEveryThingGameMode_Game>(GetWorld()->GetAuthGameMode());
		if (GameMode && GameMode->CanTakeDamage(this, OtherActor))
		{
			float Damage = 0.f;
			FPointDamageEvent DamageEvent;
			DamageEvent.HitInfo = Hit;
			IHitAbleInterface* HitableActor = Cast<IHitAbleInterface>(OtherActor);
			if (HitableActor)
			{
				AGamePawn* OtherPawn = Cast<AGamePawn>(OtherActor);
				if (OtherPawn)
				{
					Damage = GameMode->GetDamageFromGamePawnHit(this, OtherPawn, NormalInpulse, Hit);
					DamageEvent.Damage = Damage;
					OtherPawn->TakeDamage(Damage, DamageEvent, GetController(), this);
				}
				else
				{
					Damage = GameMode->GetDamageFromHitableHit(this, NormalInpulse, Hit);
					DamageEvent.Damage = Damage;
					OtherActor->TakeDamage(Damage, DamageEvent, GetController(), this);
				}
			}
			else
			{
				Damage = GameMode->GetDamageFromActorHit(this, NormalInpulse, Hit);
				DamageEvent.Damage = Damage;
				TakeDamage(Damage, DamageEvent, GetController(), OtherActor);
			}
		}
	}
}
void AGamePawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	OnHitImplement(HitComp, OtherActor, OtherComp, NormalInpulse, Hit);
	OnHitDelegate.Broadcast(HitComp, OtherActor, OtherComp, NormalInpulse, Hit);
}

void AGamePawn::OnHitImplement(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	AcceptHitFrom(OtherActor, NormalInpulse, Hit);
}

void AGamePawn::SpendStamina(float value) { if (HasAuthority()) { Stamina -= value; } }

void AGamePawn::ChangeStaminaTick(float DeltaTime)
{
	if (Stamina < MaxStamina)
	{
		Stamina += StaminaRecoverRate*DeltaTime;
	}
}

void AGamePawn::ChangeStaminaRecovery(float Force)
{
	StaminaRecoverRate -= Force*0.001;
}

void AGamePawn::ResetStaminaRecovery()
{
	StaminaRecoverRate = MaxStamina / 6.f;
}

bool AGamePawn::ServerChangeDurability_Validate(float value) { return true; }
void AGamePawn::ServerChangeDurability_Implementation(float value)
{
	ChangeDurability(GetDurability() + value);
}
void AGamePawn::ChangeDurability(float value) { if (HasAuthority()) { Durability = value; } }


float AGamePawn::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ServerChangeDurability(-FinalDamage);
	return FinalDamage;
}


//////////////////////////////////////////////////////////////////////////
/// Game Pawn Form
void AGamePawn::ToggleToNewFormWithIndex(int32 Index) { if (AllRoleFormName.IsValidIndex(Index)) { ToggleToTargetForm(AllRoleFormName[Index]); } }

bool AGamePawn::ServerToggleToTargetForm_Validate(const FName& FormName) { return true; }
void AGamePawn::ServerToggleToTargetForm_Implementation(const FName& FormName) { ToggleToTargetForm(FormName); }
void AGamePawn::OnRep_CurrentRoleFormName() { ToggleToTargetForm(CurrentRoleFormName); }

void AGamePawn::ToggleToTargetForm(const FName& FormName)
{
	if (!AllHaveRoleFormName.Contains(FormName))
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("FormName"), FText::FromName(FormName));
		OnToggleToTargetSkinFailureDelegate.Broadcast(FormName, FText::Format(LOCTEXT("ToggleSkinWhenNotHaveTargetForm", "Don't have Target form name : {FormName}."), Arguments));
		return;
	}

	if (FormName == CurrentRoleFormName)
	{
		OnToggleToTargetFormFailureDelegate.Broadcast(FormName, LOCTEXT("ToggleSkinWhenSameForm", "Already is the target Form, needn't to toggle."));
		return;
	}
	
	if (Role == ROLE_AutonomousProxy && !HasAuthority())
	{
		ServerToggleToTargetForm(FormName);
	}

	if (CurrentRoleForm) { CurrentRoleForm->UnloadGamePawnForm(); delete CurrentRoleForm; }
	

	FRoleForm* TargetGamePawnForm = UGamePawnManager::CreateRoleForm(FormName, this);

	CurrentRoleFormName = FormName;
	CurrentRoleForm = TargetGamePawnForm;

	if (CurrentRoleForm) { CurrentRoleForm->LoadGamePawnForm(); }

	OnToggleToTargetSkinSuccessDelegate.Broadcast(FormName);
}

//////////////////////////////////////////////////////////////////////////
/// Game Pawn Skin

void AGamePawn::ToggleToNewSkinWithIndex(int32 Index) { if (AllRoleSkinName.IsValidIndex(Index)) { ToggleToTargetSkin(AllRoleSkinName[Index]); } }
bool AGamePawn::ServerToggleToTargetSkin_Validate(const FName& SkinName) { return true; }
void AGamePawn::ServerToggleToTargetSkin_Implementation(const FName& SkinName) { ToggleToTargetSkin(SkinName); }
void AGamePawn::OnRep_CurrentRoleSkinName() { ToggleToTargetSkin(CurrentRoleSkinName); }

void AGamePawn::ToggleToTargetSkin(const FName& SkinName)
{
	if (!AllHaveRoleSkinName.Contains(SkinName))
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("SkinName"), FText::FromName(SkinName));
		OnToggleToTargetSkinFailureDelegate.Broadcast(SkinName, FText::Format(LOCTEXT("ToggleSkinWhenNotHaveTargetSkin", "Don't have Target skin name : {SkinName}."), Arguments));
		return;
	}

	if (SkinName == CurrentRoleSkinName)
	{
		OnToggleToTargetSkinFailureDelegate.Broadcast(SkinName, LOCTEXT("ToggleSkinWhenSameSkin", "Already is the target Skin, needn't to toggle."));
		return;
	}
	

	if (Role == ROLE_AutonomousProxy && !HasAuthority())
	{
		ServerToggleToTargetSkin(SkinName);
	}
	
	if (CurrentRoleSkin) { CurrentRoleSkin->UnloadGamePawnSkin(); delete CurrentRoleSkin; }

	FRoleSkin* TargetGamePawnSkin = UGamePawnManager::CreateRoleSkin(SkinName, StaticMeshComp);
	CurrentRoleSkinName = SkinName;
	CurrentRoleSkin = TargetGamePawnSkin;

	if (CurrentRoleSkin) { CurrentRoleSkin->LoadGamePawnSkin(); }

	OnToggleToTargetSkinSuccessDelegate.Broadcast(SkinName);
}

//////////////////////////////////////////////////////////////////////////
/// Attack and Skill
void AGamePawn::ToggleToNewAttackComponent(UAttackComponent* InAttackComponent)
{
	OwnerAttackComp = InAttackComponent;

	APlayerController_Game* OwnerPlayerPC = Cast<APlayerController_Game>(GetController());
	if (OwnerPlayerPC) { OwnerPlayerPC->ToggleToNewAttackComponent(OwnerAttackComp); }
}

void AGamePawn::ToggleToNewSkillComponent(USkillComponent* InSkillComponent)
{
	OwnerSkillComp = InSkillComponent;

	APlayerController_Game* OwnerPlayerPC = Cast<APlayerController_Game>(GetController());
	if (OwnerPlayerPC) { OwnerPlayerPC->ToggleToNewSkillComponent(OwnerSkillComp); }
}

void AGamePawn::OnRep_OwnerAttackComp() { ToggleToNewAttackComponent(OwnerAttackComp); }
void AGamePawn::OnRep_OwnerSkillComp() { ToggleToNewSkillComponent(OwnerSkillComp); }


AActor* AGamePawn::TryToGetAttackTarget(float InMaxAttackDistance)
{
	IGamePawnControllerInterface* OwnerPawnGPC = Cast<IGamePawnControllerInterface>(GetController());

	AActor* AttackTarget = OwnerPawnGPC ? OwnerPawnGPC->GetAttackTarget() : nullptr;

	if (AttackTarget)
	{
		if ((AttackTarget->GetActorLocation() - GetActorLocation()).Size() < InMaxAttackDistance)
		{
			FCollisionQueryParams TraceParams;
			TraceParams.AddIgnoredActor(this);
			FHitResult Hit;
		
			GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), AttackTarget->GetActorLocation(), ECC_Visibility, TraceParams);

			if (Hit.GetActor() == nullptr || Hit.GetActor() == AttackTarget)
			{
				return AttackTarget;
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("-_- can't attack target actor, because has a block object"));
			}
		}	
		else
		{
			UE_LOG(LogTemp, Log, TEXT("-_- exceed the max attack distance"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("-_- not found attack target"));
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
/// Game Pawn info

void AGamePawn::ResetInfoFromDataTable(const FName& GamePawnName)
{
	UDataTable* GamePawnInfoDataDable = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName("GamePawn");
	if (GamePawnInfoDataDable)
	{
		FGamePawnInfo* GamePawnInfo = GamePawnInfoDataDable->FindRow<FGamePawnInfo>(GamePawnName, FString::Printf(TEXT("find game pawn %s base info"), *GamePawnName.ToString()));
		if (GamePawnInfo) { SetInfo(GamePawnInfo); }
	}
}

void AGamePawn::SetInfo(const FGamePawnInfo* InInfo)
{
	OwnerInfo = *InInfo;
	MaxDurability = InInfo->MaxDurability;
	MaxStamina = InInfo->MaxStamina;
	Durability = MaxDurability;
	Stamina = MaxStamina;
}

void AGamePawn::UpdateInfo()
{
	ResetQuality();
	ResetDamping();

	if (MovementComp) { MovementComp->UpdateAgilityAndQuality(OwnerInfo.Agility, OwnerInfo.Quality, OwnerInfo.QualityScale); }
}

void AGamePawn::OnRep_Info()
{
	UpdateInfo();
}

//////////////////////////////////////////////////////////////////////////
/// Quality And Damping
void AGamePawn::SetQualityScale(float InQualityScale) { OwnerInfo.QualityScale = InQualityScale; ResetQuality(); }

void AGamePawn::ResetQuality()
{
	FBodyInstance* BodyInstance = StaticMeshComp->GetBodyInstance();
	if (BodyInstance)
	{
		BodyInstance->MassScale = OwnerInfo.QualityScale;
		BodyInstance->SetMassOverride(OwnerInfo.Quality);
		BodyInstance->UpdateMassProperties();
	}
}
void AGamePawn::ResetDamping()
{
	FBodyInstance* BodyInstance = StaticMeshComp->GetBodyInstance();
	if(BodyInstance)
	{
		BodyInstance->LinearDamping  = OwnerInfo.LinearDamping;
		BodyInstance->AngularDamping = OwnerInfo.AngularDamping;
		BodyInstance->UpdateDampingProperties();
	}
}

void AGamePawn::ResetDefaultSkinAndFormFromDataTable()
{
	const FRoleInfo* RoleInfo;
	if (UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetRoleInfoFromName(RoleName, RoleInfo) && RoleInfo)
	{
		AllHaveRoleSkinName.AddUnique(RoleInfo->DefaultSkinName);
		OnAllHaveRoleSkinNameUpdate();
		ToggleToTargetSkin(RoleInfo->DefaultSkinName);
		
		AllHaveRoleFormName.AddUnique(RoleInfo->DefaultFormName);
		OnAllHaveRoleFormNameUpdate();
		ToggleToTargetForm(RoleInfo->DefaultFormName);
	}
}




void AGamePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePawn, CurrentRoleSkinName);
	DOREPLIFETIME(AGamePawn, CurrentRoleFormName);

	DOREPLIFETIME(AGamePawn, OwnerAttackComp);
	DOREPLIFETIME(AGamePawn, OwnerSkillComp);

	DOREPLIFETIME(AGamePawn, OwnerInfo);
	DOREPLIFETIME(AGamePawn, Durability);
	DOREPLIFETIME(AGamePawn, Stamina);
}


#undef LOCTEXT_NAMESPACE