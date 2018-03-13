// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameHUD.h"

#include "UI/Game/GameMenu.h"
#include "UI/Game/GameLayout.h"
#include "EveryThingAssetManager.h"

#include "ChatWindow/UI/ChatWindow.h"
#include "ChatWIndow/UI/ChatLine.h"


void AEveryThingGameHUD::BeginPlay()
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




void AEveryThingGameHUD::DisplayGameMenu()
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
		if (bIsDisplay) { GameLayout->DisplaySelectRolesBox(); }
		else { GameLayout->RemoveSelectRolesBox(); }
	}
}

void AEveryThingGameHUD::ToggleSelectSkinsBox(bool bIsDisplay)
{
	if (GameLayout)
	{
		if (bIsDisplay) { GameLayout->DisplaySelectSkinsBox(); }
		else { GameLayout->RemoveSelectSkinsBox(); }
	}
}

void AEveryThingGameHUD::ToggleSelectFormsBox(bool bIsDisplay)
{
	if (GameLayout)
	{
		if (bIsDisplay) { GameLayout->DisplaySelectFormsBox(); }
		else { GameLayout->RemoveSelectFormsBox(); }
	}
}


//////////////////////////////////////////////////////////////////////////
/// For Chat Window HUD Interface

void AEveryThingGameHUD::InitializeChatWindow()
{
	if (ChatWindow) { ChatWindow->InitializeChatWindow(); }
}

void AEveryThingGameHUD::FocusToChatWindow()
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

void AEveryThingGameHUD::RemoveChatWidnowFocus()
{
	APlayerController* OwnerPC = GetOwningPlayerController();
	if (OwnerPC)
	{
		OwnerPC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		OwnerPC->SetInputMode(InputMode);
	}
}

void AEveryThingGameHUD::FocusToChatInput() { if (ChatWindow) { ChatWindow->FocusToChatInput(); } }
void AEveryThingGameHUD::FocusToChatReply() { if (ChatWindow) { ChatWindow->FocusToChatReply(); } }
void AEveryThingGameHUD::FocusToChatCommand() { if (ChatWindow) { ChatWindow->FocusToChatCommand(); } }

TSubclassOf<UUserWidget> AEveryThingGameHUD::GetChatLineWidgetClass()
{
	return UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("ChatLine");
}
TSubclassOf<UUserWidget> AEveryThingGameHUD::GetChatWindowWidgetClass()
{
	return UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("ChatWindow");
}

void AEveryThingGameHUD::ReceiveChatMessage(const FChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage)
{
	if (ChatWindow) { ChatWindow->ReceiveChatMessage(ChatChannel, ChatMessage); }
}
