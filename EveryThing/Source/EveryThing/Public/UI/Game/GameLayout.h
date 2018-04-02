// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Online/EveryThingGameState_Game.h"

#include "GameLayout.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UGameLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeGameLayout();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateGameUIDisplay(EETGameState GameUIState);

	void DisplaySelectRolesBox();
	void RemoveSelectRolesBox();

	void DisplaySelectSkinsBox();
	void RemoveSelectSkinsBox();
	
	void DisplaySelectFormsBox();
	void RemoveSelectFormsBox();
public:
	UPROPERTY(BlueprintReadWrite, Transient)
	class USelectRolesBox* SelectRolesBox;
	UPROPERTY(BlueprintReadWrite, Transient)
	class USelectSkinsBox* SelectSkinsBox;
	UPROPERTY(BlueprintReadWrite, Transient)
	class USelectFormsBox* SelectFormsBox;
	UPROPERTY(BlueprintReadWrite, Transient)
	class UImportantNoteBox* ImportNoteBox;
	UPROPERTY(BlueprintReadWrite, Transient)
	class UChatWindow* ChatWindow;
	UPROPERTY(BlueprintReadWrite, Transient)
	class UUserInfoBox* UserInfoBox;
	UPROPERTY(BlueprintReadWrite, Transient)
	class UReadyCountDown* ReadyCountDown;
	UPROPERTY(BlueprintReadWrite, Transient)
	class UGameInfoOnTop* GameInfoOnTop;
};
