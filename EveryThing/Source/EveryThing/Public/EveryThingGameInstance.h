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

class UEveryThingAssetManager;

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

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameUI")
	TSubclassOf<class UMainMenu> MainMenuClass;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	class UMainMenu* MainMenu;

	UPROPERTY(EditDefaultsOnly, Category = "GameUI")
	TSubclassOf<class UHouseList> HouseListClass;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	class UHouseList* HouseList;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GameUI")
	TSubclassOf<class ULoadingScreen> LoadingScreenClass;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	class ULoadingScreen* LoadingScreen;

	UPROPERTY(EditDefaultsOnly, Category = "GameUI")
	TSubclassOf<class UErrorDialog> ErrorDialogClass;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GameUI")
	class UErrorDialog* ErrorDialog;
};
