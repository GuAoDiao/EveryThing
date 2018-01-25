// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingMenuHUD.h"

#include "EveryThingGameInstance.h"
#include "EveryThingAssetManager.h"

#include "UI/ErrorDialog.h"
#include "UI/MainMenu.h"
#include "UI/HouseList.h"
#include "UI/HouseRow.h"
#include "UI/HouseCreate.h"
#include "UI/LoadingScreen.h"


AEveryThingMenuHUD::AEveryThingMenuHUD()
{
	CurrentGameUIState = EGameUIState::StartUp;

	UEveryThingAssetManager*  OwnerAssetManager = UEveryThingAssetManager::GetAssetManagerInstance();
	

	MainMenuClass = OwnerAssetManager->GetUserWidgetFromName(TEXT("MainMenu"));
	ErrorDialogClass = OwnerAssetManager->GetUserWidgetFromName(TEXT("ErrorDialog"));
	HouseListClass = OwnerAssetManager->GetUserWidgetFromName(TEXT("HouseList"));
	HouseCreateClass = OwnerAssetManager->GetUserWidgetFromName(TEXT("HouseCreate"));
	LoadingScreenClass = OwnerAssetManager->GetUserWidgetFromName(TEXT("LoadingScreen"));
}

void AEveryThingMenuHUD::BeginPlay()
{
	BeginPlayShowMainMenu();
}

void AEveryThingMenuHUD::ToggleToNewGameUIState(EGameUIState InGameUIState)
{
	switch (CurrentGameUIState)
	{
		case EGameUIState::StartUp:
			break;
		case EGameUIState::MainMenu:
			if (MainMenu) { MainMenu->RemoveFromParent(); }
			break;
		case EGameUIState::HouseCreate:
			if (HouseCreate) { HouseCreate->RemoveFromParent(); }
			break;
		case EGameUIState::HouseList:
			if (HouseList) { HouseList->RemoveFromParent(); }
			break;
		case EGameUIState::LoadingScreen:
			if (LoadingScreen) { LoadingScreen->RemoveFromParent(); }
			break;
		case EGameUIState::ErrorDialog:
			if (ErrorDialog) { ErrorDialog->RemoveFromParent(); }
			break;
		case EGameUIState::Playing:
			break;
		case EGameUIState::Unknown:
			break;
		default:
			break;
	}

	CurrentGameUIState = InGameUIState;

	switch (CurrentGameUIState)
	{
		case EGameUIState::StartUp:
			break;
		case EGameUIState::MainMenu:
		{
			if (IsTargetGameUIState(EGameUIState::Playing))
			{
				UEveryThingGameInstance* OwnerInstance = Cast<UEveryThingGameInstance>(GetGameInstance());
				if (!OwnerInstance) { return; }
				OwnerInstance->OpenMenuLevel();
			}
			ShowMainMenu();
			break;
		}
		case EGameUIState::HouseCreate:
			ShowHouseCreate();
			break;
		case EGameUIState::HouseList:
			ShowHouseList();
			break;
		case EGameUIState::LoadingScreen:
			ShowLoadingScreen();
			break;
		case EGameUIState::ErrorDialog:
			break;
		case EGameUIState::Playing:
			break;
		case EGameUIState::Unknown:
			break;
		default:
			break;
	}
}

void AEveryThingMenuHUD::BeginPlayShowMainMenu()
{
	if (IsTargetGameUIState(EGameUIState::StartUp)) { ToggleToNewGameUIState(EGameUIState::MainMenu); }
}

//////////////////////////////////////////////////////////////////////////
/// UI

void AEveryThingMenuHUD::ShowMainMenu()
{
	if (IsTargetGameUIState(EGameUIState::MainMenu))
	{
		if (!MainMenu && MainMenuClass)
		{
			MainMenu = CreateWidget<UMainMenu>(GetGameInstance(), MainMenuClass);
		}

		if (MainMenu)
		{
			MainMenu->AddToViewport();
			SetWidgetOwnerAndInputModeToFocusWidget(MainMenu);

			APlayerController* OwnerPC = GetGameInstance()->GetFirstLocalPlayerController();
			if (OwnerPC) { OwnerPC->bShowMouseCursor = true; }
		}
	}
}

void AEveryThingMenuHUD::ShowHouseCreate()
{
	if (IsTargetGameUIState(EGameUIState::HouseCreate))
	{
		if (!HouseCreate && HouseCreateClass)
		{
			HouseCreate = CreateWidget<UHouseCreate>(GetGameInstance(), HouseCreateClass);
		}

		if (HouseCreate)
		{
			HouseCreate->AddToViewport();
			SetWidgetOwnerAndInputModeToFocusWidget(HouseCreate);
		}
	}
}

void AEveryThingMenuHUD::ShowHouseList()
{
	if (IsTargetGameUIState(EGameUIState::HouseList))
	{
		if (!HouseList && HouseListClass)
		{
			HouseList = CreateWidget<UHouseList>(GetGameInstance(), HouseListClass);
		}

		if (HouseList)
		{
			HouseList->AddToViewport();
			SetWidgetOwnerAndInputModeToFocusWidget(HouseList);
		}
	}
}

void AEveryThingMenuHUD::ShowLoadingScreen()
{
	if (IsTargetGameUIState(EGameUIState::LoadingScreen))
	{
		if (!LoadingScreen && LoadingScreenClass)
		{
			LoadingScreen = CreateWidget<ULoadingScreen>(GetGameInstance(), LoadingScreenClass);
		}

		if (LoadingScreen)
		{
			LoadingScreen->AddToViewport();
			SetWidgetOwnerAndInputModeToFocusWidget(LoadingScreen);
		}
	}
}

void AEveryThingMenuHUD::ShowErrorDialog(const FString& ErrorMessage)
{
	if (IsTargetGameUIState(EGameUIState::ErrorDialog))
	{

		if (!ErrorDialog && ErrorDialogClass)
		{
			ErrorDialog = CreateWidget<UErrorDialog>(GetGameInstance(), ErrorDialogClass);
		}

		if (ErrorDialog)
		{
			ErrorDialog->SetErrorMessage(FText::FromString(ErrorMessage));
			ErrorDialog->AddToViewport();

			SetWidgetOwnerAndInputModeToFocusWidget(ErrorDialog);
		}
	}
}

void AEveryThingMenuHUD::UpdateHouseList(TArray<FOnlineSessionSearchResult>& SearchResults)
{
	if (IsTargetGameUIState(EGameUIState::HouseList) && HouseList)
	{
		HouseList->UpdateHouseList(SearchResults);
	}
}


void AEveryThingMenuHUD::SetWidgetOwnerAndInputModeToFocusWidget(UUserWidget* InWidget)
{
	APlayerController* OwnerPC = GetGameInstance()->GetFirstLocalPlayerController();
	if (OwnerPC && InWidget)
	{
		InWidget->SetOwningPlayer(OwnerPC);

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(InWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		OwnerPC->SetInputMode(InputMode);
	}
}
