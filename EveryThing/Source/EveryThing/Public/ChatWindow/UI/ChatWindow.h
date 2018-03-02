// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ChatWindowTypes.h"

#include "ChatWindow.generated.h"

class UChatComponent;
class UChatInput;
class UChatChannel;

/**
 * 
 */
UCLASS()
class EVERYTHING_API UChatWindow : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent)
	void InitializeChatWindow();

	//////////////////////////////////////////////////////////////////////////
	/// Chat Component
public:
	UChatComponent* GetChatComponent() const { return ChatComponent; }
	
protected:
	UPROPERTY(Transient)
	UChatComponent* ChatComponent;

	//////////////////////////////////////////////////////////////////////////
	/// Chat Channel
public:
	UChatChannel* GetChatChannel() const { return ChatChannel; }
	bool ToggleChatChannel(TSoftClassPtr<UChatChannel> InChatChannel);

	void ToggleToReplayWithPlayerID(int32 PlayerID);

protected:
	UChatChannel* ChatChannel;

	//////////////////////////////////////////////////////////////////////////
	/// Chat input
public:
	UFUNCTION(BlueprintCallable)
	void FocusChatInput();
	UFUNCTION(BlueprintCallable)
	void FocusChatCommand();
	UFUNCTION(BlueprintCallable)
	void FocusChatReply();

	UFUNCTION(BlueprintCallable)
	void RemoveChatFocus();
protected:
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere)
	UChatInput* ChatInput;
	//////////////////////////////////////////////////////////////////////////
	/// Chat Line
public:
	void ReceiveChatMessage(class UChatChannel* InChatChannel, const FChatMessageInfo& ChatMessage);

	UFUNCTION(BlueprintImplementableEvent)
	void AddChatLine(class UChatLine* ChatLine);
};
