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
	CreateArchive,
	ArchiveList,
	HouseMenu,
	HouseCreate,
	HouseList,
	Config,
	LoadingScreen,
	ErrorDialog,
	Unknown
};


class UHouseMenu;
class UHouseList;
class UHouseCreate;
class ULoadingScreen;
class UErrorDialog;
class UMainMenu;
class UCreateArchive;
class UArchiveList;

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
	UFUNCTION(BlueprintCallable)
	void ToggleToLastGameUIState();
private:
	void FinishOldGameUIState(EMenuUIState InGameUIState);
	void StartNewGameUIState(EMenuUIState InGameUIState);
private:
	EMenuUIState CurrentGameUIState;
	EMenuUIState LastGameUIState;

	//////////////////////////////////////////////////////////////////////////
	/// UI
	
public:
	void SetErrorDialogMessage(const FString& ErrorMessage);
	void UpdateHouseList(TArray<FOnlineSessionSearchResult>& SearchResults);
	
private:
	void ShowMainMenu();
	void ShowCreateArchive();
	void ShowArchiveList();
	void ShowHouseMenu();
	void ShowHouseCreate();
	void ShowHouseList();
	void ShowLoadingScreen();
	void ShowErrorDialog();

	void SetWidgetOwnerAndInputModeToFocusWidget(class UUserWidget* InWidget);

protected:
	TSubclassOf<UHouseMenu> HouseMenuClass;
	TSubclassOf<UErrorDialog> ErrorDialogClass;
	TSubclassOf<UHouseList> HouseListClass;
	TSubclassOf<UHouseCreate> HouseCreateClass;
	TSubclassOf<ULoadingScreen> LoadingScreenClass;
	TSubclassOf<UMainMenu> MainMenuClass;
	TSubclassOf<UCreateArchive> CreateArchiveClass;
	TSubclassOf<UArchiveList> ArchiveListClass;

	UPROPERTY(Transient)
	UMainMenu* MainMenu;
	UPROPERTY(Transient)
	UCreateArchive* CreateArchive;
	UPROPERTY(Transient)
	UArchiveList* ArchiveList;
	UPROPERTY(Transient)
	UHouseMenu* HouseMenu;
	UPROPERTY(Transient)
	UHouseList* HouseList;
	UPROPERTY(Transient)
	UHouseCreate* HouseCreate;
	UPROPERTY(Transient)
	ULoadingScreen* LoadingScreen;
	UPROPERTY(Transient)
	UErrorDialog* ErrorDialog;	
};
