// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatComponent.h"

#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"

#include "ChatWindow/ChatWindowHUDInterface.h"

UChatComponent::UChatComponent()
{
}


IChatWindowHUDInterface* UChatComponent::GetChatWindowHUD() const
{
	APlayerController* OwnerPC = Cast<APlayerController>(GetOwner());
	return OwnerPC ? Cast<IChatWindowHUDInterface>(OwnerPC->GetHUD()) : nullptr;
}

void UChatComponent::ClientReceiveChatMessage_Implementation(UChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage)
{
	IChatWindowHUDInterface* OwnerCWH = GetChatWindowHUD();
	if (OwnerCWH) { OwnerCWH->ReceiveChatMessage(ChatChannel, ChatMessage); }
}

bool UChatComponent::ServerSendChatMessage_Validate(UChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage) { return true; }
void UChatComponent::ServerSendChatMessage_Implementation(UChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage)
{

}