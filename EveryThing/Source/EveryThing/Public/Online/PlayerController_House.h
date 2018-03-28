// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_House.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API APlayerController_House : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void StartGameWhenIsHouseOwner();

	UFUNCTION(BlueprintCallable)
	void ApplyHouseSetting();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerApplyHouseSetting();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartGameWhenIsHouseOwner();

	virtual void OnRep_PlayerState() override;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateUpdateDelegate, APlayerState* /* PlayerState */)
	FOnPlayerStateUpdateDelegate OnPlayerStateUpdateDelegate;

};
