// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatChannel.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class EVERYTHING_API UChatChannel : public UObject
{
	GENERATED_BODY()

public:
	UChatChannel();

	virtual bool IsSwitchable() { return true; }
	
	virtual FText GetDisplayChannelText() const { return FText::FromString(FString::Printf(*DisplayChannelTextFormat, *ChannelName.ToString())); }
	virtual FText GetDiaplayNameText(const FString& PlayerName) const { return FText::FromString(FString::Printf(*DisplayChannelTextFormat, *PlayerName)); }
	virtual const FName& GetChannaelName() const { return ChannelName; }
	virtual const FLinearColor GetDisplayColor() const { return FLinearColor::Red; }
protected:
	FString DisplayChannelTextFormat;
	FString DisplayNameTextFormat;
	FName ChannelName;
};
