// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameInstance.h"

#include "Kismet/GameplayStatics.h"


#include "Online/EveryThingGameMode.h"
#include "Online/EveryThingGameSession.h"

#include "UI/ErrorDialog.h"
#include "UI/MainMenu.h"
#include "UI/HouseList.h"
#include "UI/LoadingScreen.h"


UEveryThingGameInstance::UEveryThingGameInstance()
{
	CurrentGameUIState = EGameUIState::StartUp;
}

bool UEveryThingGameInstance::IsToggleToNewGameUIState(EGameUIState InGameUIState)
{
	if (CurrentGameUIState == InGameUIState)
	{
		return false;
	}
	CurrentGameUIState = InGameUIState;
	return true;
}

void UEveryThingGameInstance::ToggleToNewGameUIState(EGameUIState InGameUIState)
{
	switch (InGameUIState)
	{
		case EGameUIState::StartUp:
			break;
		case EGameUIState::MainMenu:
			if (MainMenu) { MainMenu->RemoveFromParent(); }
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
}

void UEveryThingGameInstance::HostGame()
{
	ShowLoadingScreen();

	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		if(UserId.IsValid()){}

		FName SessionName = FName(*FString(UserId->ToString() + TEXT("_Game")));

		GetGameSession()->HostSession(*UserId, SessionName, TEXT(""), TEXT(""), true, true, 8);

	}
}


AEveryThingGameSession* UEveryThingGameInstance::GetGameSession() const
{
	AEveryThingGameMode* OwnerETGM = GetWorld() ? GetWorld()->GetAuthGameMode<AEveryThingGameMode>() : nullptr;
	return OwnerETGM ? Cast<AEveryThingGameSession>(OwnerETGM->GameSession) : nullptr;
}

//////////////////////////////////////////////////////////////////////////
/// Level
void UEveryThingGameInstance::OpenGameLevel()
{
	UGameplayStatics::OpenLevel(this, GameLevelName, true, TEXT("listen"));
}

void UEveryThingGameInstance::OpenMenuLevel()
{
	UGameplayStatics::OpenLevel(this, MenuLevelName, true);
}

//////////////////////////////////////////////////////////////////////////
/// UI

void UEveryThingGameInstance::ShowMainMenu()
{
	if (IsTargetGameUIState(EGameUIState::Playing)) { OpenMenuLevel(); }

	if (IsToggleToNewGameUIState(EGameUIState::HouseList))
	{
		if (!MainMenu && MainMenuClass)
		{
			MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
		}

		if (MainMenu)
		{
			MainMenu->AddToViewport();
			SetWidgetOwnerAndInputModeToFocusWidget(MainMenu);

			APlayerController* OwnerPC = GetFirstLocalPlayerController();
			if (OwnerPC) { OwnerPC->bShowMouseCursor = true; }
		}
	}
}

void UEveryThingGameInstance::ShowHouseList()
{
	if (IsToggleToNewGameUIState(EGameUIState::HouseList))
	{
		if (!HouseList && HouseListClass)
		{
			HouseList = CreateWidget<UHouseList>(this, HouseListClass);
		}

		if (HouseList)
		{
			HouseList->AddToViewport();
			SetWidgetOwnerAndInputModeToFocusWidget(HouseList);
		}
	}
}

void UEveryThingGameInstance::ShowLoadingScreen()
{
	if (IsToggleToNewGameUIState(EGameUIState::LoadingScreen))
	{
		if (!LoadingScreen && LoadingScreenClass)
		{
			LoadingScreen = CreateWidget<ULoadingScreen>(this, LoadingScreenClass);
		}

		if (LoadingScreen)
		{
			LoadingScreen->AddToViewport();
			SetWidgetOwnerAndInputModeToFocusWidget(LoadingScreen);
		}
	}
}

void UEveryThingGameInstance::ShowErrorDialog(const FString& ErrorMessage)
{
	if (IsToggleToNewGameUIState(EGameUIState::ErrorDialog))
	{

		if (!ErrorDialog && ErrorDialogClass)
		{
			ErrorDialog = CreateWidget<UErrorDialog>(this, ErrorDialogClass);
		}

		if (ErrorDialog)
		{
			ErrorDialog->SetErrorMessage(FText::FromString(ErrorMessage));
			ErrorDialog->AddToViewport();

			SetWidgetOwnerAndInputModeToFocusWidget(ErrorDialog);
		}
	}
}


void UEveryThingGameInstance::SetWidgetOwnerAndInputModeToFocusWidget(UUserWidget* InWidget)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC && InWidget)
	{
		InWidget->SetOwningPlayer(PC);

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(InWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}
}
