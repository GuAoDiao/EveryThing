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
	if (IsTargetGameUIState(EGameUIState::StartUp)) { ToggleToNewGameUIState(EGameUIState::MainMenu); }
}

//////////////////////////////////////////////////////////////////////////
/// Game UI State

void AEveryThingMenuHUD::ToggleToNewGameUIState(EGameUIState InGameUIState)
{
	if (InGameUIState != EGameUIState::LoadingScreen)
	{
		FinishOldGameUIState(CurrentGameUIState);
	}

	CurrentGameUIState = InGameUIState;

	StartNewGameUIState(CurrentGameUIState);
}
void AEveryThingMenuHUD::StartNewGameUIState(EGameUIState InGameUIState)
{
	switch (InGameUIState)
	{
		case EGameUIState::StartUp:
			break;
		case EGameUIState::MainMenu:
			ShowMainMenu();
			break;
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
			ShowErrorDialog();
			break;
		case EGameUIState::Unknown:
			break;
	}
}
void AEveryThingMenuHUD::FinishOldGameUIState(EGameUIState InGameUIState)
{
	switch (InGameUIState)
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
		case EGameUIState::Unknown:
			break;
	}
}


//////////////////////////////////////////////////////////////////////////
/// UI


void AEveryThingMenuHUD::SetErrorDialogMessage(const FString& ErrorMessage)
{
	if (IsTargetGameUIState(EGameUIState::ErrorDialog) && ErrorDialog)
	{
		ErrorDialog->SetErrorMessage(FText::FromString(ErrorMessage));
	}
}

void AEveryThingMenuHUD::UpdateHouseList(TArray<FOnlineSessionSearchResult>& SearchResults)
{
	if (IsTargetGameUIState(EGameUIState::LoadingScreen) && HouseList)
	{
		HouseList->UpdateHouseList(SearchResults);
		ToggleToNewGameUIState(EGameUIState::HouseList);
	}
}


void AEveryThingMenuHUD::ShowMainMenu()
{
	if (IsTargetGameUIState(EGameUIState::MainMenu))
	{
		if (!MainMenu && MainMenuClass) { MainMenu = CreateWidget<UMainMenu>(GetGameInstance(), MainMenuClass); }

		if (MainMenu)
		{
			if (!MainMenu->IsInViewport()) { MainMenu->AddToViewport(); }

			SetWidgetOwnerAndInputModeToFocusWidget(MainMenu);

			APlayerController* OwnerPC = GetGameInstance() ? GetGameInstance()->GetFirstLocalPlayerController() : nullptr;
			if (OwnerPC) { OwnerPC->bShowMouseCursor = true; }
		}
	}
}

void AEveryThingMenuHUD::ShowHouseCreate()
{
	if (IsTargetGameUIState(EGameUIState::HouseCreate))
	{
		if (!HouseCreate && HouseCreateClass) { HouseCreate = CreateWidget<UHouseCreate>(GetGameInstance(), HouseCreateClass); }

		if (HouseCreate)
		{
			if (!HouseCreate->IsInViewport()) { HouseCreate->AddToViewport(); }
			SetWidgetOwnerAndInputModeToFocusWidget(HouseCreate);
		}
	}
}

void AEveryThingMenuHUD::ShowHouseList()
{
	if (IsTargetGameUIState(EGameUIState::HouseList))
	{
		if (!HouseList && HouseListClass) { HouseList = CreateWidget<UHouseList>(GetGameInstance(), HouseListClass); }

		if (HouseList)
		{
			if (!HouseList->IsInViewport()) { HouseList->AddToViewport(); }
			SetWidgetOwnerAndInputModeToFocusWidget(HouseList);
		}
	}
}

void AEveryThingMenuHUD::ShowLoadingScreen()
{
	if (IsTargetGameUIState(EGameUIState::LoadingScreen))
	{
		if (!LoadingScreen && LoadingScreenClass) { LoadingScreen = CreateWidget<ULoadingScreen>(GetGameInstance(), LoadingScreenClass); }

		if (LoadingScreen)
		{
			if (!LoadingScreen->IsInViewport()) { LoadingScreen->AddToViewport(); }
			SetWidgetOwnerAndInputModeToFocusWidget(LoadingScreen);
		}
	}
}

void AEveryThingMenuHUD::ShowErrorDialog()
{
	if (IsTargetGameUIState(EGameUIState::ErrorDialog))
	{

		if (!ErrorDialog && ErrorDialogClass) { ErrorDialog = CreateWidget<UErrorDialog>(GetGameInstance(), ErrorDialogClass); }

		if (ErrorDialog)
		{
			if (!ErrorDialog->IsInViewport()) { ErrorDialog->AddToViewport(); }
			SetWidgetOwnerAndInputModeToFocusWidget(ErrorDialog);
		}
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
