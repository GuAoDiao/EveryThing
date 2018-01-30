// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CreateHouse();

	UFUNCTION(BlueprintCallable)
	void HouseList();

	UFUNCTION(BlueprintCallable)
	void Config();

	UFUNCTION(BlueprintCallable)
	void ExitGame();	
};
