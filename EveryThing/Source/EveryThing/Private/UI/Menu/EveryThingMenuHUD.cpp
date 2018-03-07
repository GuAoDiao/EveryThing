// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingMenuHUD.h"

#include "EveryThingGameInstance.h"
#include "EveryThingAssetManager.h"

#include "UI/Menu/MainMenu.h"

#include "UI/Menu/Archive/CreateArchive.h"
#include "UI/Menu/Archive/ArchiveList.h"

#include "UI/Menu/MasterInterface.h"

#include "UI/Menu/Storehouse/Storehouse.h"

#include "UI/Menu/House/HouseMenu.h"
#include "UI/Menu/House/HouseList.h"
#include "UI/Menu/House/HouseRow.h"
#include "UI/Menu/House/HouseCreate.h"

#include "UI/Menu/LoadingScreen.h"
#include "UI/Menu/ErrorDialog.h"


AEveryThingMenuHUD::AEveryThingMenuHUD()
{
	CurrentGameUIState = EMenuUIState::StartUp;
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

		case EMenuUIState::MasterInterface:
			ShowMasterInterface();
			break;
		case EMenuUIState::Storehouse:
			ShowStorehouse();
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

		case EMenuUIState::MasterInterface:
			if (MasterInterface) { MasterInterface->RemoveFromParent(); }
			break;
		case EMenuUIState::Storehouse:
			if (Storehouse) { Storehouse->RemoveFromParent(); }
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


template<typename T>
T* AEveryThingMenuHUD::CreateAndDisplayWidget(EMenuUIState InNeededUIState, const FName& InUserWidgetName, T* ResultWidget)
{
	if (IsTargetGameUIState(InNeededUIState))
	{
		TSubclassOf<UUserWidget> TargetClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName(InUserWidgetName);
		if (!ResultWidget && TargetClass) { ResultWidget = CreateWidget<T>(GetGameInstance(), TargetClass); }

		if (ResultWidget)
		{
			if (!ResultWidget->IsInViewport()) { ResultWidget->AddToViewport(); }

			SetWidgetOwnerAndInputModeToFocusWidget(ResultWidget);
		}
	}

	return ResultWidget;
}

void AEveryThingMenuHUD::ShowMainMenu() { MainMenu = CreateAndDisplayWidget<UMainMenu>(EMenuUIState::MainMenu, "MainMenu", MainMenu); }

void AEveryThingMenuHUD::ShowCreateArchive() { CreateArchive = CreateAndDisplayWidget<UCreateArchive>(EMenuUIState::CreateArchive, "CreateArchive", CreateArchive); }
void AEveryThingMenuHUD::ShowArchiveList() { ArchiveList = CreateAndDisplayWidget<UArchiveList>(EMenuUIState::ArchiveList, "ArchiveList", ArchiveList); }

void AEveryThingMenuHUD::ShowMasterInterface() { MasterInterface = CreateAndDisplayWidget<UMasterInterface>(EMenuUIState::MasterInterface, "MasterInterface", MasterInterface); }
void AEveryThingMenuHUD::ShowStorehouse() { Storehouse = CreateAndDisplayWidget<UStorehouse>(EMenuUIState::Storehouse, "Storehouse", Storehouse); }

void AEveryThingMenuHUD::ShowHouseMenu() { HouseMenu = CreateAndDisplayWidget<UHouseMenu>(EMenuUIState::HouseMenu, "HouseMenu", HouseMenu); }
void AEveryThingMenuHUD::ShowHouseCreate() { HouseCreate = CreateAndDisplayWidget<UHouseCreate>(EMenuUIState::HouseCreate, "HouseCreate", HouseCreate); }
void AEveryThingMenuHUD::ShowHouseList() { HouseList = CreateAndDisplayWidget<UHouseList>(EMenuUIState::HouseList, "HouseList", HouseList); }

void AEveryThingMenuHUD::ShowLoadingScreen() { LoadingScreen = CreateAndDisplayWidget<ULoadingScreen>(EMenuUIState::LoadingScreen, "LoadingScreen", LoadingScreen); }
void AEveryThingMenuHUD::ShowErrorDialog() { ErrorDialog = CreateAndDisplayWidget<UErrorDialog>(EMenuUIState::ErrorDialog, "ErrorDialog", ErrorDialog); }

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
