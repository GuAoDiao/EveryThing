// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOver.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UGameOver : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRemaningBackToHouseTime(float RemaningBackToHouseTime);

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeGameOver(int32 GetGold);
	
};
