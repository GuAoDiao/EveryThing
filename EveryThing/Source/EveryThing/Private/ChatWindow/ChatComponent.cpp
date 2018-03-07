// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatComponent.h"

#include "Engine/World.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerController.h"


#include "ChatWindow/Channel/SystemChatChannel.h"
#include "ChatWindow/Channel/AllChatChannel.h"
#include "ChatWindow/Channel/TeamChatChannel.h"


#include "ChatWindow/ChatWindowControllerInterface.h"
#include "ChatWindow/ChatWindowGameStateInterface.h"
#include "ChatWindow/ChatWindowPlayerStateInterface.h"
#include "ChatWindow/ChatWindowHUDInterface.h"

UChatComponent::UChatComponent()
{
	ChannelManager.AddChatchannelDefaultValue(new FAllChatChannel(), "Default");

	ChannelManager.AddChatchannelDefaultValue(new FAllChatChannel(), "All");
	ChannelManager.AddChatchannelDefaultValue(new FTeamChatChannel(), "Team");
	ChannelManager.AddChatchannelDefaultValue(new FSystemChatChannel(), "System");
}


IChatWindowHUDInterface* UChatComponent::GetChatWindowHUD() const
{
	APlayerController* OwnerPC = Cast<APlayerController>(GetOwner());
	return OwnerPC ? Cast<IChatWindowHUDInterface>(OwnerPC->GetHUD()) : nullptr;
}

IChatWindowPlayerStateInterface* UChatComponent::GetChatWindowPlayerState() const
{
	APlayerController* OwnerPC = Cast<APlayerController>(GetOwner());
	return OwnerPC ? Cast<IChatWindowPlayerStateInterface>(OwnerPC->PlayerState) : nullptr;
}

IChatWindowGameStateInterface* UChatComponent::GetChatWindowGameState() const
{

	UWorld* World = GetOwner() ? GetOwner()->GetWorld() : nullptr;
	return World ? Cast<IChatWindowGameStateInterface>(World->GetGameState()) : nullptr;
}

void UChatComponent::ReceiveChatMessage(const FChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage)
{
	IChatWindowHUDInterface* OwnerCWH = GetChatWindowHUD();
	if (OwnerCWH) { OwnerCWH->ReceiveChatMessage(ChatChannel, ChatMessage); }
}
void UChatComponent::ClientReceiveChatMessage_Implementation(const FName& ChatChannelName, const FChatMessageInfo& ChatMessage)
{
	FChatChannel* ChatChannel = GetChannelManager().GetChatchannelDefaultValueFromName(ChatChannelName);
	if (ChatChannel)
	{
		ReceiveChatMessage(ChatChannel, ChatMessage);
	}
}


void UChatComponent::CreateChatMessage(const FChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage)
{
	if (ChatChannel) { ServerCreateChatMessage(ChatChannel->GetChannaelName(), ChatMessage); }
}

bool UChatComponent::ServerCreateChatMessage_Validate(const FName& ChatChannelName, const FChatMessageInfo& ChatMessage) { return true; }
void UChatComponent::ServerCreateChatMessage_Implementation(const FName& ChatChannelName, const FChatMessageInfo& ChatMessage)
{
	IChatWindowGameStateInterface* OwnerCWGS = GetChatWindowGameState();
	FChatChannel* ChatChannel = GetChannelManager().GetChatchannelDefaultValueFromName(ChatChannelName);
	if (OwnerCWGS && ChatChannel)
	{
		for (APlayerState* CurrentPS : OwnerCWGS->GetAllChatPlayerState())
		{
			if (ChatChannel->IsNeededToSend(CurrentPS))
			{
				IChatWindowControllerInterface* CurrentCWC = Cast<IChatWindowControllerInterface>(CurrentPS->GetOwner());
				UChatComponent* CurrentChatComponent = CurrentCWC ? CurrentCWC->GetChatComponent() : nullptr;
				if (CurrentChatComponent)
				{
					CurrentChatComponent->ClientReceiveChatMessage(ChatChannel->GetChannaelName(), ChatMessage);
				}
			}
		}
	}
}