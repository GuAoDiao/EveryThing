// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingHUD_Game.h"

#include "UI/Game/GameMenu.h"
#include "UI/Game/GameLayout.h"
#include "EveryThingAssetManager.h"

#include "ChatWindow/UI/ChatWindow.h"
#include "ChatWIndow/UI/ChatLine.h"


void AEveryThingHUD_Game::BeginPlay()
{
	TSubclassOf<UUserWidget> GameLayoutClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName(TEXT("GameLayout"));
	if (GameLayoutClass)
	{
		GameLayout = CreateWidget<UGameLayout>(GetOwningPlayerController(), GameLayoutClass);
		
		if (GameLayout)
		{
			GameLayout->InitializeGameLayout();
			ChatWindow = GameLayout->ChatWindow;
			InitializeChatWindow();

			GameLayout->AddToViewport();
		}
	}
}




void AEveryThingHUD_Game::DisplayGameMenu()
{
	if (!GameMenu)
	{
		TSubclassOf<UUserWidget> GameMenuClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName(TEXT("GameMenu"));
		if (GameMenuClass)
		{
			GameMenu = CreateWidget<UGameMenu>(GetOwningPlayerController(), GameMenuClass);
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
void AEveryThingHUD_Game::RemoveGameMenu()
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


void AEveryThingHUD_Game::ToggleSelectRolesBox(bool bIsDisplay)
{
	if (GameLayout)
	{
		if (bIsDisplay) { GameLayout->DisplaySelectRolesBox(); }
		else { GameLayout->RemoveSelectRolesBox(); }
	}
}

void AEveryThingHUD_Game::ToggleSelectSkinsBox(bool bIsDisplay)
{
	if (GameLayout)
	{
		if (bIsDisplay) { GameLayout->DisplaySelectSkinsBox(); }
		else { GameLayout->RemoveSelectSkinsBox(); }
	}
}

void AEveryThingHUD_Game::ToggleSelectFormsBox(bool bIsDisplay)
{
	if (GameLayout)
	{
		if (bIsDisplay) { GameLayout->DisplaySelectFormsBox(); }
		else { GameLayout->RemoveSelectFormsBox(); }
	}
}


//////////////////////////////////////////////////////////////////////////
/// For Chat Window HUD Interface

void AEveryThingHUD_Game::InitializeChatWindow()
{
	if (ChatWindow) { ChatWindow->InitializeChatWindow(); }
}

void AEveryThingHUD_Game::FocusToChatWindow()
{
	if (ChatWindow)
	{
		APlayerController* OwnerPC = GetOwningPlayerController();
		if (OwnerPC)
		{
			OwnerPC->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(ChatWindow->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			OwnerPC->SetInputMode(InputMode);
		}

		FocusToChatInput();
	}
}

void AEveryThingHUD_Game::RemoveChatWidnowFocus()
{
	APlayerController* OwnerPC = GetOwningPlayerController();
	if (OwnerPC)
	{
		OwnerPC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		OwnerPC->SetInputMode(InputMode);
	}
}

void AEveryThingHUD_Game::FocusToChatInput() { if (ChatWindow) { ChatWindow->FocusToChatInput(); } }
void AEveryThingHUD_Game::FocusToChatReply() { if (ChatWindow) { ChatWindow->FocusToChatReply(); } }
void AEveryThingHUD_Game::FocusToChatCommand() { if (ChatWindow) { ChatWindow->FocusToChatCommand(); } }

TSubclassOf<UUserWidget> AEveryThingHUD_Game::GetChatLineWidgetClass()
{
	return UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("ChatLine");
}
TSubclassOf<UUserWidget> AEveryThingHUD_Game::GetChatWindowWidgetClass()
{
	return UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("ChatWindow");
}

void AEveryThingHUD_Game::ReceiveChatMessage(const FChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage)
{
	if (ChatWindow) { ChatWindow->ReceiveChatMessage(ChatChannel, ChatMessage); }
}