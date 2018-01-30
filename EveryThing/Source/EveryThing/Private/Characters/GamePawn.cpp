#include "GamePawn.h"

#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

#include "EveryThingAssetManager.h"
#include "Online/EveryThingGameMode.h"
#include "Characters/GamePawnForm.h"
#include "Characters/GamePawnSkin.h"
#include "Characters/Moves/AttackComponent.h"
#include "Characters/Moves/SkillComponent.h"
#include "Characters/PlayerPawnController.h"
#include "Characters/Movement/Components/GamePawnMovementComponent.h"



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

void AGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGamePawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (FGamePawnForm* Form : OwnerGamePawnForms)
	{
		delete Form;
	}
	for (FGamePawnSkin* Skin : OwnerGamePawnSkins)
	{
		delete Skin;
	}
	OwnerGamePawnForms.Empty();
	OwnerGamePawnSkins.Empty();

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
void AGamePawn::AddGamePawnForm(FGamePawnForm* InGamePawnForm) { if (InGamePawnForm) { OwnerGamePawnForms.Add(InGamePawnForm); } }

void AGamePawn::ToggleToNewPawnForm(int32 Index) { ServerToggleToNewPawnForm(Index); }

bool AGamePawn::ServerToggleToNewPawnForm_Validate(int32 Index) { return true; }
void AGamePawn::ServerToggleToNewPawnForm_Implementation(int32 Index)
{
	FGamePawnForm* TargetPawnForm = GetGamePawnForm(Index);
	if (TargetPawnForm && TargetPawnForm != CurrentGamePawnForm)
	{
		CurrentGamePawnFormIndex = Index;
		ToggleToTargetPawnForm(TargetPawnForm);
		UE_LOG(LogTemp, Log, TEXT("-_- Toggle To %d Pawn Form"), Index)
	}
}
void AGamePawn::OnRep_CurrentGamePawnFormIndex()
{
	FGamePawnForm* TargetPawnForm = GetGamePawnForm(CurrentGamePawnFormIndex);
	if (TargetPawnForm && TargetPawnForm != CurrentGamePawnForm)
	{
		ToggleToTargetPawnForm(TargetPawnForm);
		UE_LOG(LogTemp, Log, TEXT("-_- Toggle To %d Pawn Form"), CurrentGamePawnFormIndex)
	}
}

void AGamePawn::ToggleToTargetPawnForm(FGamePawnForm* TargetGamePawnForm)
{
	if (CurrentGamePawnForm) { CurrentGamePawnForm->UnloadGamePawnForm();}

	CurrentGamePawnForm = TargetGamePawnForm;

	if (CurrentGamePawnForm) { CurrentGamePawnForm->LoadGamePawnForm(); }
}

FGamePawnForm* AGamePawn::GetGamePawnForm(int32 Index)
{
	return OwnerGamePawnForms.IsValidIndex(Index) ? OwnerGamePawnForms[Index] : nullptr;
}

//////////////////////////////////////////////////////////////////////////
/// Game Pawn Skin

void AGamePawn::AddGamePawnSkin(FGamePawnSkin* InGamePawnSkin) { if (InGamePawnSkin) { OwnerGamePawnSkins.Add(InGamePawnSkin); } }

void AGamePawn::ToggleToNewPawnSkin(int32 Index, bool bIsCauser)
{
	if (bIsCauser && !HasAuthority())
	{
		ServerToggleToNewPawnSkin(Index);
	}

	FGamePawnSkin* TargetPawnSkin = GetGamePawnSkin(Index);
	if (TargetPawnSkin && TargetPawnSkin != CurrentGamePawnSkin)
	{
		CurrentGamePawnSkinIndex = Index;
		ToggleToTargetPawnSkin(TargetPawnSkin);
		UE_LOG(LogTemp, Log, TEXT("-_- Toggle To %d Pawn Skin"), Index)
	}
}
bool AGamePawn::ServerToggleToNewPawnSkin_Validate(int32 Index) { return true; }
void AGamePawn::ServerToggleToNewPawnSkin_Implementation(int32 Index) { ToggleToNewPawnSkin(Index); }
void AGamePawn::OnRep_CurrentGamePawnSkinIndex() { ToggleToNewPawnSkin(CurrentGamePawnSkinIndex, false); }

void AGamePawn::ToggleToTargetPawnSkin(FGamePawnSkin* TargetGamePawnSkin)
{
	if (CurrentGamePawnSkin) { CurrentGamePawnSkin->UnloadGamePawnSkin(); }

	CurrentGamePawnSkin = TargetGamePawnSkin;

	if (CurrentGamePawnSkin) { CurrentGamePawnSkin->LoadGamePawnSkin(); }
}

FGamePawnSkin* AGamePawn::GetGamePawnSkin(int32 Index) { return OwnerGamePawnSkins.IsValidIndex(Index) ? OwnerGamePawnSkins[Index] : nullptr; }

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
	UpdateInfo();
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
	StaticMeshComp->SetMassOverrideInKg(NAME_None, OwnerInfo.Quality);
	StaticMeshComp->SetMassScale(NAME_None, OwnerInfo.QualityScale);
}
void AGamePawn::ResetDamping()
{
	StaticMeshComp->SetLinearDamping(OwnerInfo.LinearDamping);
	StaticMeshComp->SetAngularDamping(OwnerInfo.AngularDamping);
}




void AGamePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePawn, CurrentGamePawnSkinIndex);
	DOREPLIFETIME(AGamePawn, CurrentGamePawnFormIndex);

	DOREPLIFETIME(AGamePawn, OwnerAttackComp);
	DOREPLIFETIME(AGamePawn, OwnerSkillComp);

	DOREPLIFETIME(AGamePawn, OwnerInfo);
}