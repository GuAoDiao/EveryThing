// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingMenuHUD.h"

#include "EveryThingGameInstance.h"
#include "EveryThingAssetManager.h"

#include "UI/Menu/MainMenu.h"
#include "UI/Menu/ErrorDialog.h"
#include "UI/Menu/HouseMenu.h"
#include "UI/Menu/HouseList.h"
#include "UI/Menu/HouseRow.h"
#include "UI/Menu/HouseCreate.h"
#include "UI/Menu/CreateArchive.h"
#include "UI/Menu/ArchiveList.h"
#include "UI/Menu/LoadingScreen.h"


AEveryThingMenuHUD::AEveryThingMenuHUD()
{
	CurrentGameUIState = EMenuUIState::StartUp;

	UEveryThingAssetManager*  OwnerAssetManager = UEveryThingAssetManager::GetAssetManagerInstance();
	
	MainMenuClass = OwnerAssetManager->GetUserWidgetFromName("MainMenu");
	CreateArchiveClass = OwnerAssetManager->GetUserWidgetFromName("CreateArchive");
	ArchiveListClass = OwnerAssetManager->GetUserWidgetFromName("ArchiveList");
	HouseMenuClass = OwnerAssetManager->GetUserWidgetFromName("HouseMenu");
	ErrorDialogClass = OwnerAssetManager->GetUserWidgetFromName("ErrorDialog");
	HouseListClass = OwnerAssetManager->GetUserWidgetFromName("HouseList");
	HouseCreateClass = OwnerAssetManager->GetUserWidgetFromName("HouseCreate");
	LoadingScreenClass = OwnerAssetManager->GetUserWidgetFromName("LoadingScreen");
}

void AEveryThingMenuHUD::BeginPlay()
{
	if (IsTargetGameUIState(EMenuUIState::StartUp)) { ToggleToNewGameUIState(EMenuUIState::MainMenu); }

	if (GetOwningPlayerController()) { GetOwningPlayerController()->bShowMouseCursor = true; }
}

//////////////////////////////////////////////////////////////////////////
/// Game UI State

void AEveryThingMenuHUD::ToggleToNewGameUIState(EMenuUIState InGameUIState)
{
	if (InGameUIState != EMenuUIState::LoadingScreen)
	{
		FinishOldGameUIState(CurrentGameUIState);
	}

	LastGameUIState = CurrentGameUIState;
	CurrentGameUIState = InGameUIState;

	StartNewGameUIState(CurrentGameUIState);
}
void AEveryThingMenuHUD::ToggleToLastGameUIState()
{
	ToggleToNewGameUIState(LastGameUIState);
}

void AEveryThingMenuHUD::StartNewGameUIState(EMenuUIState InGameUIState)
{
	switch (InGameUIState)
	{
		case EMenuUIState::StartUp:
			break;
		case EMenuUIState::MainMenu:
			ShowMainMenu();
			break;
		case EMenuUIState::CreateArchive:
			ShowCreateArchive();
			break;
		case EMenuUIState::ArchiveList:
			ShowArchiveList();
			break;
		case EMenuUIState::HouseMenu:
			ShowHouseMenu();
			break;
		case EMenuUIState::HouseCreate:
			ShowHouseCreate();
			break;
		case EMenuUIState::HouseList:
			ShowHouseList();
			break;
		case EMenuUIState::LoadingScreen:
			ShowLoadingScreen();
			break;
		case EMenuUIState::ErrorDialog:
			ShowErrorDialog();
			break;
		case EMenuUIState::Unknown:
			break;
	}
}
void AEveryThingMenuHUD::FinishOldGameUIState(EMenuUIState InGameUIState)
{
	switch (InGameUIState)
	{
		case EMenuUIState::StartUp:
			break;
		case EMenuUIState::MainMenu:
			if (MainMenu) { MainMenu->RemoveFromParent(); }
			break;
		case EMenuUIState::CreateArchive:
			if (CreateArchive) {CreateArchive->RemoveFromParent();}
			break;
		case EMenuUIState::ArchiveList:
			if (ArchiveList) {ArchiveList->RemoveFromParent();}
			break;
		case EMenuUIState::HouseMenu:
			if (HouseMenu) { HouseMenu->RemoveFromParent(); }
			break;
		case EMenuUIState::HouseCreate:
			if (HouseCreate) { HouseCreate->RemoveFromParent(); }
			break;
		case EMenuUIState::HouseList:
			if (HouseList) { HouseList->RemoveFromParent(); }
			break;
		case EMenuUIState::LoadingScreen:
			if (LoadingScreen) { LoadingScreen->RemoveFromParent(); }
			break;
		case EMenuUIState::ErrorDialog:
			if (ErrorDialog) { ErrorDialog->RemoveFromParent(); }
			break;
		case EMenuUIState::Unknown:
			break;
	}
}


//////////////////////////////////////////////////////////////////////////
/// UI


void AEveryThingMenuHUD::SetErrorDialogMessage(const FString& ErrorMessage)
{
	if (IsTargetGameUIState(EMenuUIState::ErrorDialog) && ErrorDialog)
	{
		ErrorDialog->SetErrorMessage(FText::FromString(ErrorMessage));
	}
}

void AEveryThingMenuHUD::UpdateHouseList(TArray<FOnlineSessionSearchResult>& SearchResults)
{
	if (IsTargetGameUIState(EMenuUIState::LoadingScreen) && HouseList)
	{
		HouseList->UpdateHouseList(SearchResults);
		ToggleToNewGameUIState(EMenuUIState::HouseList);
	}
}


void AEveryThingMenuHUD::ShowMainMenu()
{
	if (IsTargetGameUIState(EMenuUIState::MainMenu))
	{
		if (!MainMenu && HouseMenuClass) { MainMenu = CreateWidget<UMainMenu>(GetGameInstance(), MainMenuClass); }

		if (MainMenu)
		{
			if (!MainMenu->IsInViewport()) { MainMenu->AddToViewport(); }

			SetWidgetOwnerAndInputModeToFocusWidget(MainMenu);
		}
	}
}

void AEveryThingMenuHUD::ShowCreateArchive()
{
	if (IsTargetGameUIState(EMenuUIState::CreateArchive))
	{
		if (!CreateArchive && HouseMenuClass) { CreateArchive = CreateWidget<UCreateArchive>(GetGameInstance(), CreateArchiveClass); }

		if (CreateArchive)
		{
			if (!CreateArchive->IsInViewport()) { CreateArchive->AddToViewport(); }

			SetWidgetOwnerAndInputModeToFocusWidget(CreateArchive);
		}
	}
}

void AEveryThingMenuHUD::ShowArchiveList()
{
	if (IsTargetGameUIState(EMenuUIState::ArchiveList))
	{
		if (!ArchiveList && ArchiveListClass) { ArchiveList = CreateWidget<UArchiveList>(GetGameInstance(), ArchiveListClass); }

		if (ArchiveList)
		{
			if (!ArchiveList->IsInViewport()) { ArchiveList->AddToViewport(); }

			SetWidgetOwnerAndInputModeToFocusWidget(ArchiveList);
		}
	}
}

void AEveryThingMenuHUD::ShowHouseMenu()
{
	if (IsTargetGameUIState(EMenuUIState::HouseMenu))
	{
		if (!HouseMenu && HouseMenuClass) { HouseMenu = CreateWidget<UHouseMenu>(GetGameInstance(), HouseMenuClass); }

		if (HouseMenu)
		{
			if (!HouseMenu->IsInViewport()) { HouseMenu->AddToViewport(); }

			SetWidgetOwnerAndInputModeToFocusWidget(HouseMenu);
		}
	}
}

void AEveryThingMenuHUD::ShowHouseCreate()
{
	if (IsTargetGameUIState(EMenuUIState::HouseCreate))
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
	if (IsTargetGameUIState(EMenuUIState::HouseList))
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
	if (IsTargetGameUIState(EMenuUIState::LoadingScreen))
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
	if (IsTargetGameUIState(EMenuUIState::ErrorDialog))
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
