// Fill out your copyright notice in the Description page of Project Settings.

#include "ChannelManager.h"

#include "ChatWindow/Channel/ChatChannel.h"


void FChannelManager::AddChatchannelClass(TSoftClassPtr<UChatChannel> ChatChanelClass, const FName& ChatChannelName)
{
	AllChatChannels.Add(ChatChannelName, ChatChanelClass);
}

TSoftClassPtr<UChatChannel> FChannelManager::GetChatchannelClass(const FName& ChatChannelName) const
{
	if (AllChatChannels.Contains(ChatChannelName))
	{
		return AllChatChannels[ChatChannelName];
	}
	return nullptr;
}