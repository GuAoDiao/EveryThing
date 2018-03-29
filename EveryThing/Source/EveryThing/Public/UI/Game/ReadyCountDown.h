// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadyCountDown.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UReadyCountDown : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateReadyCountDownTime(float Time);
};
