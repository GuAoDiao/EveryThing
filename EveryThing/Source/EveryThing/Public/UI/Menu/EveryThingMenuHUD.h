// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "OnlineSessionSettings.h"

#include "EveryThingMenuHUD.generated.h"


UENUM(BlueprintType)
enum class EMenuUIState : uint8
{
	StartUp,
	MainMenu,
	HouseCreate,
	HouseList,
	Config,
	LoadingScreen,
	ErrorDialog,
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
	/// Game UI State
public:
	UFUNCTION(BlueprintPure)
	EMenuUIState GetCurrentGameUIState() const { return CurrentGameUIState; }
	UFUNCTION(BlueprintPure)
	bool IsTargetGameUIState(EMenuUIState InGameUIState) const { return InGameUIState == CurrentGameUIState; }
	UFUNCTION(BlueprintCallable)
	void ToggleToNewGameUIState(EMenuUIState InGameUIState);
private:
	void FinishOldGameUIState(EMenuUIState InGameUIState);
	void StartNewGameUIState(EMenuUIState InGameUIState);
private:
	EMenuUIState CurrentGameUIState;

	//////////////////////////////////////////////////////////////////////////
	/// UI
	
public:
	void SetErrorDialogMessage(const FString& ErrorMessage);
	void UpdateHouseList(TArray<FOnlineSessionSearchResult>& SearchResults);
	
private:
	void ShowMainMenu();
	void ShowHouseCreate();
	void ShowHouseList();
	void ShowLoadingScreen();
	void ShowErrorDialog();

	void SetWidgetOwnerAndInputModeToFocusWidget(class UUserWidget* InWidget);

protected:
	TSubclassOf<UMainMenu> MainMenuClass;
	TSubclassOf<UErrorDialog> ErrorDialogClass;
	TSubclassOf<UHouseList> HouseListClass;
	TSubclassOf<UHouseCreate> HouseCreateClass;
	TSubclassOf<ULoadingScreen> LoadingScreenClass;

	UMainMenu* MainMenu;
	UHouseList* HouseList;
	UHouseCreate* HouseCreate;
	ULoadingScreen* LoadingScreen;
	UErrorDialog* ErrorDialog;	
};