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
	void ToggleGameMenu();
	void DisplayGameMenu();
	UFUNCTION(BlueprintCallable)
	void RemoveGameMenu();
private:
	class UGameMenu* GameMenu;
};
