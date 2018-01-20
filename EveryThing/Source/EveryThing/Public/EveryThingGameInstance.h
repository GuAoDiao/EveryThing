// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EveryThingGameInstance.generated.h"

UENUM(BlueprintType)
enum class EGameUIState : uint8
{
	StartUp,
	MainMenu,
	HouseList,
	LoadingScreen,
	ErrorDialog,
	Playing,
	Unknown
};

/**
 * 
 */
UCLASS()
class EVERYTHING_API UEveryThingGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UEveryThingGameInstance();

	//////////////////////////////////////////////////////////////////////////
	/// Game State
public:
	UFUNCTION(BlueprintPure)
	EGameUIState GetCurrentGameUIState() const { return CurrentGameUIState; }
	UFUNCTION(BlueprintPure)
	bool IsTargetGameUIState(EGameUIState InGameUIState) const { return InGameUIState == CurrentGameUIState; }
	UFUNCTION(BlueprintCallable)
	void ToggleToNewGameUIState(EGameUIState InGameUIState);

private:
	bool IsToggleToNewGameUIState(EGameUIState InGameUIState);
private:
	EGameUIState CurrentGameUIState;
	
	//////////////////////////////////////////////////////////////////////////
	/// Game Match
public:
	void HostGame();


private:
	class AEveryThingGameSession* GetGameSession() const;
	//////////////////////////////////////////////////////////////////////////
	/// Level
public:
	UFUNCTION(BlueprintCallable)
	void OpenMenuLevel();
	UFUNCTION(BlueprintCallable)
	void OpenGameLevel();

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName GameLevelName;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName MenuLevelName;


	//////////////////////////////////////////////////////////////////////////
	/// UI
public:
	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();
	UFUNCTION(BlueprintCallable)
	void ShowHouseList();
	UFUNCTION(BlueprintCallable)
	void ShowLoadingScreen();
	UFUNCTION(BlueprintCallable)
	void ShowErrorDialog(const FString& ErrorMessage);


private:
	void SetWidgetOwnerAndInputModeToFocusWidget(class UUserWidget* InWidget);
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UMainMenu> MainMenuClass;
	UPROPERTY(BlueprintReadOnly)
	class UMainMenu* MainMenu;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UHouseList> HouseListClass;
	UPROPERTY(BlueprintReadOnly)
	class UHouseList* HouseList;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class ULoadingScreen> LoadingScreenClass;
	UPROPERTY(BlueprintReadOnly)
	class ULoadingScreen* LoadingScreen;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UErrorDialog> ErrorDialogClass;
	UPROPERTY(BlueprintReadOnly)
	class UErrorDialog* ErrorDialog;
};
