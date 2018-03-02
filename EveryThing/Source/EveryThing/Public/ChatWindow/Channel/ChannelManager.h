// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChannelManager.generated.h"


class UChatChannel;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FChannelManager
{
	GENERATED_USTRUCT_BODY()
public:
	TMap<FName, TSoftClassPtr<UChatChannel>> AllChatChannels;

	void AddChatchannelClass(TSoftClassPtr<UChatChannel> ChatChanel, const FName& ChatChannelName = NAME_None);

	TSoftClassPtr<UChatChannel> GetChatchannelClass(const FName& ChatChannelName) const;
};
