// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameHUD.h"

#include "UI/Game/GameMenu.h"
#include "EveryThingAssetManager.h"

void AEveryThingGameHUD::ToggleGameMenu()
{
	if (!GameMenu || !GameMenu->IsInViewport())
	{
		DisplayGameMenu();
	}
	else
	{
		RemoveGameMenu();
	}
}

void AEveryThingGameHUD::DisplayGameMenu()
{
	if (!GameMenu)
	{
		TSubclassOf<UUserWidget> GameMenuClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName(TEXT("GameMenu"));
		if (GameMenuClass)
		{
			GameMenu = CreateWidget<UGameMenu>(GetGameInstance(), GameMenuClass);
		}
	}

	if (GameMenu)
	{
		GameMenu->AddToViewport();

		APlayerController* OwnerPC = GetGameInstance()->GetFirstLocalPlayerController();
		if (OwnerPC)
		{
			OwnerPC->bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(GameMenu->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			OwnerPC->SetInputMode(InputMode);
		}
	}
}
void AEveryThingGameHUD::RemoveGameMenu()
{
	if (GameMenu)
	{
		GameMenu->RemoveFromParent();

		APlayerController* OwnerPC = GetGameInstance()->GetFirstLocalPlayerController();
		if (OwnerPC)
		{
			OwnerPC->bShowMouseCursor = false;

			FInputModeGameOnly InputMode;
			OwnerPC->SetInputMode(InputMode);
		}
	}
}