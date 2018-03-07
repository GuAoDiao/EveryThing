// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamChatChannel.h"


FTeamChatChannel::FTeamChatChannel()
{
	ChannelName = "Team";
	DisplayNameTextFormat = TEXT("%s Says : ");
}


FLinearColor FTeamChatChannel::GetDisplayLineColor() const { return FLinearColor(1.f, 0.1f, 0.f); }
FLinearColor FTeamChatChannel::GetDisplayChannelColor() const { return FLinearColor(1.f, 0.1f, 0.f); }