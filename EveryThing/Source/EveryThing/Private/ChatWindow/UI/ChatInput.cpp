// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatInput.h"

#include "ChatWindow/UI/ChatWindow.h"
#include "ChatWindow/ChatComponent.h"
#include "ChatWindow/Channel/ChatChannel.h"

void UChatInput::InitializeChatInput_Implementation(UChatWindow* InChatWindow)
{
	ChatWindow = InChatWindow;
}

void UChatInput::ProcessInput(const FText& InputText)
{
	FString InputString = InputText.ToString();

	FString Shortcut, Message;
	if (InputString.Split(TEXT(" "), &Shortcut, &Message, ESearchCase::IgnoreCase, ESearchDir::FromStart))
	{
		UChatComponent* ChatComponent = ChatWindow ? ChatWindow->GetChatComponent() : nullptr;
		TSoftClassPtr<UChatChannel> ChatChannel = ChatComponent ? ChatComponent->GetChannelManager().GetChatchannelClass(FName(*Shortcut)) : nullptr;
		if (ChatChannel)
		{
			if (ChatWindow->ToggleChatChannel(ChatChannel))
			{
				// remove first /
				SetInputText(FText::FromString(Message.StartsWith(TEXT("/")) ? Message.Mid(1) : Message));
			}
		}
	}
}

bool UChatInput::IsCommand(const FText& Command) const
{
	FString CommandString = Command.ToString();

	return CommandString.StartsWith(TEXT("/"), ESearchCase::IgnoreCase) && CommandString.Len() > 1;
}

void UChatInput::ExcuteInput(const FText& InputText)
{
	

	// if empty remove chat focus
	if (InputText.IsEmpty())
	{
		if (ChatWindow) { ChatWindow->RemoveChatFocus(); }
		return;
	}

	
	UChatComponent* ChatComponent = ChatWindow ? ChatWindow->GetChatComponent() : nullptr;
	if (ChatWindow && ChatComponent)
	{
		FString InputString = InputText.ToString();

		// send message or execute command
		if (InputString.StartsWith(TEXT("/"), ESearchCase::IgnoreCase))
		{
			// ChatComponent->ClientExecuteCommandEvent(InputString);
		}
		else
		{
			// ChatComponent->ClientCreateMessageEvent(ChatWindow->GetCurrentChannel(), InputString);
		}
	}

	SetInputText(FText());

	FocusToInputText();
}