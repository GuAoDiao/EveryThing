// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "ChatWindow/ChatWindowTypes.h"
#include "ChatWindowHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UChatWindowHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EVERYTHING_API IChatWindowHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/// Initialize 
	virtual void InitializeChatWindow() = 0;

	/// Chat Input
	virtual void FocusToChatInput() = 0;
	virtual void FocusToChatReply() = 0;
	virtual void FocusToChatCommand() = 0;
	virtual void RemoveChatFocus() = 0;

	virtual TSubclassOf<class UChatLine> GetChatLineWidgetClass() = 0;
	virtual TSubclassOf<class UChatWindow> GetChatWindowWidgetClass() = 0;

	// virtual void SetCurrentChannel(EChatChannel::Type ChatChannelType) = 0;

	virtual void ReceiveChatMessage(class UChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage) = 0;
};
