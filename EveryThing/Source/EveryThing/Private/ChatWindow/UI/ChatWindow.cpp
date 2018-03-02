// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatWindow.h"

#include "GameFramework/HUD.h"
#include "GameFramework/GameStateBase.h"

#include "ChatWindow/UI/ChatInput.h"
#include "ChatWindow/UI/ChatLine.h"
#include "ChatWindow/ChatWindowControllerInterface.h"
#include "ChatWindow/ChatWindowHUDInterface.h"
#include "ChatWindow/ChatWindowGameStateInterface.h"
#include "ChatWindow/Channel/ChatChannel.h"

void UChatWindow::NativeConstruct()
{
	InitializeChatWindow();


	if (ChatInput) { ChatInput->InitializeChatInput(this); }
	
	Super::NativeConstruct();
}

void UChatWindow::InitializeChatWindow_Implementation()
{
	IChatWindowControllerInterface* OwnerCWC = Cast<IChatWindowControllerInterface>(GetOwningPlayer());
	if (OwnerCWC) { ChatComponent = OwnerCWC->GetChatComponent(); }
}


/// Chat input
void UChatWindow::FocusChatInput() { if (ChatInput) { ChatInput->FocusToInputText(); } }

void UChatWindow::FocusChatCommand()
{
	if (ChatInput)
	{
		ChatInput->SetInputText(FText::FromString(TEXT("/")));
		ChatInput->FocusToInputText();
	}
}

void UChatWindow::FocusChatReply()
{
	if (ChatInput)
	{
		ChatInput->SetInputText(FText::FromString(TEXT("/r")));
		ChatInput->FocusToInputText();
	}
}

void UChatWindow::RemoveChatFocus() { SetUserFocus(GetOwningPlayer()); }

/// Chat Channel 
bool UChatWindow::ToggleChatChannel(TSoftClassPtr<UChatChannel> InChatChannel)
{
	if (InChatChannel)
	{
		UChatChannel* NewChatChannel = NewObject<UChatChannel>(nullptr, InChatChannel->GetClass());
		if (NewChatChannel)
		{
			if (NewChatChannel->IsSwitchable())
			{
				ChatChannel = NewChatChannel;
				return true;
			}
			else
			{
				NewChatChannel->BeginDestroy();
			}
		}
	}
	return false;
}

void UChatWindow::ToggleToReplayWithPlayerID(int32 PlayerID)
{

}



/// Chat Line
void UChatWindow::ReceiveChatMessage(class UChatChannel* InChatChannel, const FChatMessageInfo& ChatMessage)
{
	APlayerController* OwnerPC = GetOwningPlayer();
	if (!OwnerPC) { return; }

	IChatWindowHUDInterface* OwnerCWH = Cast<IChatWindowHUDInterface>(OwnerPC->GetHUD());
	IChatWindowGameStateInterface* OwnerCWGS = OwnerPC->GetWorld() ? Cast<IChatWindowGameStateInterface>(OwnerPC->GetWorld()->GetGameState()) : nullptr;

	if (OwnerCWH && OwnerCWGS)
	{
		IChatWindowGameStateInterface;
		TSubclassOf<UChatLine> ChatLineClass = OwnerCWH->GetChatLineWidgetClass();
		UChatLine* NewChatLine = ChatLineClass ? CreateWidget<UChatLine>(GetOwningPlayer(), ChatLineClass) : nullptr;
		if (NewChatLine)
		{
			NewChatLine->InitalizeChatLine(this, 
				InChatChannel->GetChannaelName() , InChatChannel->GetDisplayChannelText(), 
				ChatMessage.PlayerID, InChatChannel->GetDiaplayNameText(OwnerCWGS->GetPlayerChatName(ChatMessage.PlayerID)),
				FText::FromString(ChatMessage.Message), InChatChannel->GetDisplayColor());

			AddChatLine(NewChatLine);
		}
	}
}