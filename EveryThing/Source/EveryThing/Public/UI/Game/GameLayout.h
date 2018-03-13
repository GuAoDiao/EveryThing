// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameLayout.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UGameLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeGameLayout();

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
};
