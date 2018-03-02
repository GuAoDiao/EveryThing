// Fill out your copyright notice in the Description page of Project Settings.

#include "SystemChatChannel.h"

USystemChatChannel::USystemChatChannel()
{
	ChannelName = "System";
}


const FLinearColor USystemChatChannel::GetDisplayColor() const
{
	switch (MessageType)
	{
		case ESystemMessageType::Normal:
			return FLinearColor(1.f, 1.f, 1.f, 1.f);
		case ESystemMessageType::Alert:
			return FLinearColor(0.9f, 0.81f, 0.21f, 1.f);
		case ESystemMessageType::Error:
			return FLinearColor(1.f, 0.32f, 0.32f, 1.f);
		case ESystemMessageType::Team:
			return FLinearColor(0.32f, 0.5f, 1.f, 1.f);
		case ESystemMessageType::Status:
			return FLinearColor(1.f, 0.5f, 0.21f, 1.f);
		case ESystemMessageType::Experience:
			return FLinearColor(0.16f, 0.32f, 1.f, 1.f);
	}

	return FLinearColor::White;
}