// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Characters/Moves/Moves.h"

#include "MovesComponent.generated.h"


UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVERYTHING_API UMovesComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UMovesComponent();

	bool HasAuthority();

	virtual void RebindAll() {};
public:
	bool bWantedToAcceptHitFunction;
	// execute when bWantedToAcceptHitFunction is true
	virtual void OnHitImplement(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit) {};

	virtual void RebindInputComp(class UInputComponent* OwnerInputComp) {}
};
