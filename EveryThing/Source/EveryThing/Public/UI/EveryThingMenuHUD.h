// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "OnlineSessionSettings.h"

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
class UHouseRow;
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
	
public:
	void UpdateHouseList(TArray<FOnlineSessionSearchResult>& SearchResults);
private:
	void SetWidgetOwnerAndInputModeToFocusWidget(class UUserWidget* InWidget);

protected:
	TSubclassOf<UMainMenu> MainMenuClass;
	TSubclassOf<UErrorDialog> ErrorDialogClass;
	TSubclassOf<UHouseList> HouseListClass;
	TSubclassOf<UHouseCreate> HouseCreateClass;
	TSubclassOf<ULoadingScreen> LoadingScreenClass;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	UMainMenu* MainMenu;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	UHouseList* HouseList;


	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	UHouseCreate* HouseCreate;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	ULoadingScreen* LoadingScreen;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	UErrorDialog* ErrorDialog;	
};
