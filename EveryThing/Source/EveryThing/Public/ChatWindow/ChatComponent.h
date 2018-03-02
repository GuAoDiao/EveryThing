// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ChatWindow/ChatWindowTypes.h"
#include "ChatWindow/Channel/ChannelManager.h"

#include "ChatComponent.generated.h"

class IChatWindowHUDInterface;
class UChatChannel;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVERYTHING_API UChatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UChatComponent();


	IChatWindowHUDInterface* GetChatWindowHUD() const;
	
	FChannelManager& GetChannelManager() { return ChannelManager; }

protected:
	UFUNCTION(BlueprintCallable, client, Reliable)
	void ClientReceiveChatMessage(UChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerSendChatMessage(UChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage);


	FChannelManager ChannelManager;
};
