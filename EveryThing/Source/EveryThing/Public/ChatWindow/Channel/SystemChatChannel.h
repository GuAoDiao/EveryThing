// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatWindow/Channel/ChatChannel.h"

#include "EveryThingTypes.h"

#include "SystemChatChannel.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API USystemChatChannel : public UChatChannel
{
	GENERATED_BODY()
	
public:
	USystemChatChannel();	
	
	virtual const FLinearColor GetDisplayColor() const override;


	ESystemMessageType GetMessageType() const { return MessageType; }
	void SetMessageType(ESystemMessageType InMessageType) { MessageType = InMessageType; }
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESystemMessageType MessageType;
};
