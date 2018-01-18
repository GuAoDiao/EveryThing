#include "GamePawn.h"

#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"


#include "Online/EveryThingGameMode.h"
#include "Characters/GamePawnForm.h"
#include "Characters/Moves/AttackComponent.h"
#include "Characters/Moves/SkillComponent.h"
#include "Characters/Controlllers/PlayerPawnController.h"



AGamePawn::AGamePawn()
{
	bReplicates = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	
	StaticMeshComp->SetSimulatePhysics(true);

	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComp->SetCollisionObjectType(ECC_Pawn);
	StaticMeshComp->SetCollisionResponseToAllChannels(ECR_Block);
	
	StaticMeshComp->OnComponentHit.AddDynamic(this, &AGamePawn::OnHit);

	SetRootComponent(StaticMeshComp);
	
	Durability = 1000.f;
	MaxHyperopiaDistance = 10000.f;

	PrimaryActorTick.bCanEverTick = true;
}


void AGamePawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (FGamePawnForm* Form : OwnerGamePawnForms)
	{
		delete Form;
	}
	OwnerGamePawnForms.Empty();

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



void AGamePawn::ResetQuality()
{
	StaticMeshComp->SetMassScale(NAME_None, QualityScale);
	StaticMeshComp->SetMassOverrideInKg(NAME_None, Quality);
}
void AGamePawn::ResetDamping()
{
	StaticMeshComp->SetLinearDamping(LinearDamping);
	StaticMeshComp->SetAngularDamping(AngularDamping);
}
void AGamePawn::ResetQualityAndDamping()
{
	ResetQuality();
	ResetDamping();
}


bool AGamePawn::AddDurability_Validate(float InOffset, EElementType InType)
{
	return true;
}
void AGamePawn::AddDurability_Implementation(float InOffset, EElementType InType)
{
	UWorld* World = GetWorld();
	AEveryThingGameMode* OwnerGameMode = World ? World->GetAuthGameMode<AEveryThingGameMode>() : nullptr;
	if (OwnerGameMode)
	{
		Durability += OwnerGameMode->GetActualDamage(InOffset, InType, ElementType, ElementResistance);
	}
}


void AGamePawn::AddGamePawnForm(FGamePawnForm* InGamePawnForm)
{
	if (InGamePawnForm) { OwnerGamePawnForms.Add(InGamePawnForm); }
}

bool AGamePawn::ServerToggleToNewPawnFormWithIndex_Validate(int32 Index)
{
	return true;
}
void AGamePawn::ServerToggleToNewPawnFormWithIndex_Implementation(int32 Index)
{
	FGamePawnForm* TargetPawnForm = GetGamePawnForm(Index);
	if (TargetPawnForm && TargetPawnForm != CurrentGamePawnForm)
	{
		ToggleToNewPawnForm(TargetPawnForm);
		UE_LOG(LogTemp, Log, TEXT("-_- Toggle To %d Pawn Form"), Index)
	}
}

void AGamePawn::ToggleToNewPawnForm(FGamePawnForm* TargetGamePawnForm)
{
	if (CurrentGamePawnForm) { CurrentGamePawnForm->UnloadGamePawnForm();}

	CurrentGamePawnForm = TargetGamePawnForm;


	if (CurrentGamePawnForm)
	{
		CurrentGamePawnForm->LoadGamePawnForm();

		ToggleToNewAttackComponent(CurrentGamePawnForm->GetAttackComponent());
		ToggleToNewSkillComponent(CurrentGamePawnForm->GetSkillComponent());
	}
}

FGamePawnForm* AGamePawn::GetGamePawnForm(int32 Index)
{
	if (Index >= 0 && Index < OwnerGamePawnForms.Num())
	{
		return OwnerGamePawnForms[Index];
	}
	return nullptr;
}

bool AGamePawn::ToggleToNewAttackComponent_Validate(UAttackComponent* InAttackComponent)
{
	return true;
}
void AGamePawn::ToggleToNewAttackComponent_Implementation(UAttackComponent* InAttackComponent)
{
	OwnerAttackComp = InAttackComponent;

	APlayerPawnController* OwnerPlayerPC = Cast<APlayerPawnController>(GetController());
	if (OwnerPlayerPC)
	{
		OwnerPlayerPC->ToggleToNewAttackComponent(OwnerAttackComp);
	}
}

bool AGamePawn::ToggleToNewSkillComponent_Validate(USkillComponent* InSkillComponent)
{
	return true;
}
void AGamePawn::ToggleToNewSkillComponent_Implementation(USkillComponent* InSkillComponent)
{
	OwnerSkillComp = InSkillComponent;

	APlayerPawnController* OwnerPlayerPC = Cast<APlayerPawnController>(GetController());
	if (OwnerPlayerPC)
	{
		OwnerPlayerPC->ToggleToNewSkillComponent(OwnerSkillComp);
	}
}




AActor* AGamePawn::TryToGetAttackTarget(float InMaxAttackDistance)
{
	IGamePawnControllerInterface* OwnerPawnGPC = Cast<IGamePawnControllerInterface>(GetController());

	AActor* AttackTarget = OwnerPawnGPC ? OwnerPawnGPC->GetAttackTarget() : nullptr;

	if (AttackTarget)
	{
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), AttackTarget->GetActorLocation(), ECC_Visibility, TraceParams))
		{
			if (Hit.GetActor() == AttackTarget)
			{
				if ((AttackTarget->GetActorLocation() - GetActorLocation()).Size() < InMaxAttackDistance)
				{
					return AttackTarget;
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("-_- exceed the max attack distance"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("-_- can't attack target actor, because has a block object"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("-_- unknown error"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("-_- not found attack target"));
	}


	return nullptr;
}


void AGamePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePawn, Durability);
	DOREPLIFETIME(AGamePawn, PhysicalPower);
	DOREPLIFETIME(AGamePawn, Agility);
	DOREPLIFETIME(AGamePawn, Stability);
	DOREPLIFETIME(AGamePawn, PowerValue);
	DOREPLIFETIME(AGamePawn, MaxHyperopiaDistance);
}