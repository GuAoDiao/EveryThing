// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HouseLayout.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UHouseLayout : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


	void OnAddPlayer(class APlayerState* PlayerState);

	UFUNCTION(BlueprintImplementableEvent)
	void AddHousePlayerItem(class UHousePlayerItem* HousePlayerItem);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsHouseOwner(bool bIsHouserOwner);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateReadyState(bool bIsReady);

	UFUNCTION(BlueprintCallable)
	void ToggleReadState();

	UFUNCTION(BlueprintCallable)
	void StartGame();


	void OnPlayerStateUpdate(class APlayerState* PlayerState);
};
