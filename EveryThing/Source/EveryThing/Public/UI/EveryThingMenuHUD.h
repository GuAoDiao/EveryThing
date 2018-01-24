// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EveryThingMenuHUD.generated.h"


UENUM(BlueprintType)
enum class EGameUIState : uint8
{
	StartUp,
	MainMenu,
	HouseCreate,
	HouseList,
	Config,
	LoadingScreen,
	ErrorDialog,
	Playing,
	Unknown
};


class UMainMenu;
class UHouseList;
class UHouseCreate;
class ULoadingScreen;
class UErrorDialog;

/**
 * 
 */
UCLASS()
class EVERYTHING_API AEveryThingMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AEveryThingMenuHUD();
	virtual void BeginPlay() override;

	//////////////////////////////////////////////////////////////////////////
	/// Game Match
public:
	UFUNCTION(BlueprintCallable)
	void BeginPlayShowMainMenu();
	//////////////////////////////////////////////////////////////////////////
	/// Game UI State
public:
	UFUNCTION(BlueprintPure)
	EGameUIState GetCurrentGameUIState() const { return CurrentGameUIState; }
	UFUNCTION(BlueprintPure)
	bool IsTargetGameUIState(EGameUIState InGameUIState) const { return InGameUIState == CurrentGameUIState; }
	UFUNCTION(BlueprintCallable)
	void ToggleToNewGameUIState(EGameUIState InGameUIState);

private:
	EGameUIState CurrentGameUIState;

	//////////////////////////////////////////////////////////////////////////
	/// UI
private:
	void ShowMainMenu();
	void ShowHouseCreate();
	void ShowHouseList();
	void ShowLoadingScreen();
	void ShowErrorDialog(const FString& ErrorMessage);
	
private:
	void SetWidgetOwnerAndInputModeToFocusWidget(class UUserWidget* InWidget);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameUI")
	TSubclassOf<UMainMenu> MainMenuClass;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	UMainMenu* MainMenu;

	UPROPERTY(EditDefaultsOnly, Category = "GameUI")
	TSubclassOf<UHouseList> HouseListClass;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	UHouseList* HouseList;

	UPROPERTY(EditDefaultsOnly, Category = "GameUI")
	TSubclassOf<UHouseCreate> HouseCreateClass;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	UHouseCreate* HouseCreate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GameUI")
	TSubclassOf<ULoadingScreen> LoadingScreenClass;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	ULoadingScreen* LoadingScreen;

	UPROPERTY(EditDefaultsOnly, Category = "GameUI")
	TSubclassOf<UErrorDialog> ErrorDialogClass;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	UErrorDialog* ErrorDialog;	
};
