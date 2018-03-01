// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameHUD.h"

#include "UI/Game/GameMenu.h"
#include "UI/Game/GameLayout.h"
#include "EveryThingAssetManager.h"


void AEveryThingGameHUD::BeginPlay()
{
	TSubclassOf<UUserWidget> GameLayoutClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName(TEXT("GameLayout"));
	if (GameLayoutClass)
	{
		GameLayout = CreateWidget<UGameLayout>(GetGameInstance(), GameLayoutClass);
		if (GameLayout){GameLayout->AddToViewport();}
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

		APlayerController* OwnerPC = GetOwningPlayerController();
		if (OwnerPC)
		{
			OwnerPC->bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(GameMenu->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			OwnerPC->SetInputMode(InputMode);
		}
	}
}
void AEveryThingGameHUD::RemoveGameMenu()
{
	if (GameMenu)
	{
		GameMenu->RemoveFromParent();

		APlayerController* OwnerPC = GetOwningPlayerController();
		if (OwnerPC)
		{
			OwnerPC->bShowMouseCursor = false;

			FInputModeGameOnly InputMode;
			OwnerPC->SetInputMode(InputMode);
		}
	}
}


void AEveryThingGameHUD::ToggleSelectRolesBox(bool bIsDisplay)
{
	if (GameLayout)
	{
		if (bIsDisplay)
		{
			GameLayout->DisplaySelectRolesBox();
		}
		else
		{
			GameLayout->RemoveSelectRolesBox();
		}
	}
}