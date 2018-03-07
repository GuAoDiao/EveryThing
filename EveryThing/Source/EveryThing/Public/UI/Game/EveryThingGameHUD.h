// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "ChatWindow/ChatWindowHUDInterface.h"

#include "EveryThingGameHUD.generated.h"

class UChatWindow;

/**
 * 
 */
UCLASS()
class EVERYTHING_API AEveryThingGameHUD : public AHUD, public IChatWindowHUDInterface
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	//////////////////////////////////////////////////////////////////////////
	/// For Chat Window HUD Interface
public:
	/// Initialize 
	virtual void InitializeChatWindow()override;

	virtual void FocusToChatWindow() override;
	virtual void RemoveChatWidnowFocus() override;

	/// Chat Input
	virtual void FocusToChatInput() override;
	virtual void FocusToChatReply() override;
	virtual void FocusToChatCommand() override;

	virtual TSubclassOf<class UUserWidget> GetChatLineWidgetClass()override;
	virtual TSubclassOf<class UUserWidget> GetChatWindowWidgetClass()override;

	// virtual void SetCurrentChannel(EChatChannel::Type ChatChannelType) = 0;

	virtual void ReceiveChatMessage(const FChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage)override;

protected:
	UChatWindow* ChatWindow;

	//////////////////////////////////////////////////////////////////////////
	/// Game Menu
public:
	void ToggleGameMenu();

	UFUNCTION(BlueprintCallable)
	void DisplayGameMenu();
	UFUNCTION(BlueprintCallable)
	void RemoveGameMenu();

	void ToggleSelectRolesBox(bool bIsDisplay);
private:
	UPROPERTY(Transient)
	class UGameMenu* GameMenu;
	UPROPERTY(Transient)
	class UGameLayout* GameLayout;
};
