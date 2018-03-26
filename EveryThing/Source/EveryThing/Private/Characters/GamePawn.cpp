// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawn.h"

#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

#include "EveryThingAssetManager.h"
#include "Online/EveryThingGameMode.h"
#include "Characters/Form/GamePawnForm.h"
#include "Characters/Skin/GamePawnSkin.h"
#include "Characters/Moves/AttackComponent.h"
#include "Characters/Moves/SkillComponent.h"
#include "Characters/PlayerPawnController.h"
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


void AGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGamePawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurrentGamePawnSkin) { delete CurrentGamePawnSkin; }
	if (CurrentGamePawnForm) { delete CurrentGamePawnForm; }

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

void AGamePawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	OnHitImplement(HitComp, OtherActor, OtherComp, NormalInpulse, Hit);
}

void AGamePawn::OnHitImplement(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	if (OwnerAttackComp && OwnerAttackComp->bWantedToAcceptHitFunction)
	{
		OwnerAttackComp->OnHitImplement(HitComp, OtherActor, OtherComp, NormalInpulse, Hit);
	}

	if (OwnerSkillComp && OwnerSkillComp->bWantedToAcceptHitFunction)
	{
		OwnerSkillComp->OnHitImplement(HitComp, OtherActor, OtherComp, NormalInpulse, Hit);
	}
}





bool AGamePawn::AddDurability_Validate(float InOffset, EElementType InType) { return true; }
void AGamePawn::AddDurability_Implementation(float InOffset, EElementType InType)
{
	UWorld* World = GetWorld();
	AEveryThingGameMode* OwnerGameMode = World ? World->GetAuthGameMode<AEveryThingGameMode>() : nullptr;
	if (OwnerGameMode)
	{
		OwnerInfo.Durability += OwnerGameMode->GetActualDamage(InOffset, InType, OwnerInfo.ElementType[0], OwnerInfo.ElementResistance[OwnerInfo.ElementType[0]]);
	}
}

//////////////////////////////////////////////////////////////////////////
/// Game Pawn Form
void AGamePawn::ToggleToNewFormWithIndex(int32 Index) { if (AllGamePawnFormName.IsValidIndex(Index)) { ToggleToTargetForm(AllGamePawnFormName[Index]); } }

bool AGamePawn::ServerToggleToTargetForm_Validate(const FName& FormName) { return true; }
void AGamePawn::ServerToggleToTargetForm_Implementation(const FName& FormName) { ToggleToTargetForm(FormName); }
void AGamePawn::OnRep_CurrentGamePawnFormName() { ToggleToTargetForm(CurrentGamePawnFormName); }

void AGamePawn::ToggleToTargetForm(const FName& FormName)
{
	if (!AllHaveGamePawnFormName.Contains(FormName))
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("FormName"), FText::FromName(FormName));
		OnToggleToTargetSkinFailureDelegate.Broadcast(FormName, FText::Format(LOCTEXT("ToggleSkinWhenNotHaveTargetForm", "Don't have Target form name : {FormName}."), Arguments));
		return;
	}

	if (FormName == CurrentGamePawnFormName)
	{
		OnToggleToTargetFormFailureDelegate.Broadcast(FormName, LOCTEXT("ToggleSkinWhenSameForm", "Already is the target Form, needn't to toggle."));
		return;
	}
	
	if (Role == ROLE_AutonomousProxy && !HasAuthority())
	{
		ServerToggleToTargetForm(FormName);
	}

	if (CurrentGamePawnForm) { CurrentGamePawnForm->UnloadGamePawnForm();}
	delete CurrentGamePawnForm;

	FGamePawnForm* TargetGamePawnForm = UGamePawnManager::CreateGamePawnFormFromName(FormName, this);
	CurrentGamePawnFormName = FormName;
	CurrentGamePawnForm = TargetGamePawnForm;

	if (CurrentGamePawnForm) { CurrentGamePawnForm->LoadGamePawnForm(); }

	OnToggleToTargetSkinSuccessDelegate.Broadcast(FormName);
}

//////////////////////////////////////////////////////////////////////////
/// Game Pawn Skin

void AGamePawn::ToggleToNewSkinWithIndex(int32 Index) { if (AllGamePawnSkinName.IsValidIndex(Index)) { ToggleToTargetSkin(AllGamePawnSkinName[Index]); } }
bool AGamePawn::ServerToggleToTargetSkin_Validate(const FName& SkinName) { return true; }
void AGamePawn::ServerToggleToTargetSkin_Implementation(const FName& SkinName) { ToggleToTargetSkin(SkinName); }
void AGamePawn::OnRep_CurrentGamePawnSkinName() { ToggleToTargetSkin(CurrentGamePawnSkinName); }

void AGamePawn::ToggleToTargetSkin(const FName& SkinName)
{
	if (!AllHaveGamePawnSkinName.Contains(SkinName))
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("SkinName"), FText::FromName(SkinName));
		OnToggleToTargetSkinFailureDelegate.Broadcast(SkinName, FText::Format(LOCTEXT("ToggleSkinWhenNotHaveTargetSkin", "Don't have Target skin name : {SkinName}."), Arguments));
		return;
	}

	if (SkinName == CurrentGamePawnSkinName)
	{
		OnToggleToTargetSkinFailureDelegate.Broadcast(SkinName, LOCTEXT("ToggleSkinWhenSameSkin", "Already is the target Skin, needn't to toggle."));
		return;
	}
	

	if (Role == ROLE_AutonomousProxy && !HasAuthority())
	{
		ServerToggleToTargetSkin(SkinName);
	}
	
	if (CurrentGamePawnSkin) { CurrentGamePawnSkin->UnloadGamePawnSkin(); delete CurrentGamePawnSkin; }

	FGamePawnSkin* TargetGamePawnSkin = UGamePawnManager::CreateGamePawnSkinFromName(SkinName, StaticMeshComp);
	CurrentGamePawnSkinName = SkinName;
	CurrentGamePawnSkin = TargetGamePawnSkin;

	if (CurrentGamePawnSkin) { CurrentGamePawnSkin->LoadGamePawnSkin(); }

	OnToggleToTargetSkinSuccessDelegate.Broadcast(SkinName);
}

//////////////////////////////////////////////////////////////////////////
/// Attack and Skill
void AGamePawn::ToggleToNewAttackComponent(UAttackComponent* InAttackComponent)
{
	OwnerAttackComp = InAttackComponent;

	APlayerPawnController* OwnerPlayerPC = Cast<APlayerPawnController>(GetController());
	if (OwnerPlayerPC) { OwnerPlayerPC->ToggleToNewAttackComponent(OwnerAttackComp); }
}

void AGamePawn::ToggleToNewSkillComponent(USkillComponent* InSkillComponent)
{
	OwnerSkillComp = InSkillComponent;

	APlayerPawnController* OwnerPlayerPC = Cast<APlayerPawnController>(GetController());
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
		AllHaveGamePawnSkinName.Add(RoleInfo->DefaultSkinName);
		ToggleToTargetSkin(RoleInfo->DefaultSkinName);
		AllHaveGamePawnFormName.Add(RoleInfo->DefaultFormName);
		ToggleToTargetForm(RoleInfo->DefaultFormName);
	}
}




void AGamePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePawn, CurrentGamePawnSkinName);
	DOREPLIFETIME(AGamePawn, CurrentGamePawnFormName);

	DOREPLIFETIME(AGamePawn, OwnerAttackComp);
	DOREPLIFETIME(AGamePawn, OwnerSkillComp);

	DOREPLIFETIME(AGamePawn, OwnerInfo);
}


#undef LOCTEXT_NAMESPACE