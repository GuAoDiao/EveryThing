// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EveryThingGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API AEveryThingGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void ToggleGameMenu();

	UFUNCTION(BlueprintCallable)
	void DisplayGameMenu();
	UFUNCTION(BlueprintCallable)
	void RemoveGameMenu();

	void ToggleSelectRolesBox(bool bIsDisplay);
private:
	UPROPERTY(Transient)
	class UGameMenu* GameMenu;
	UPROPERTY(Transient)
	class UGameLayout* GameLayout;
};
