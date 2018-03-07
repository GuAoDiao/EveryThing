// Fill out your copyright notice in the Description page of Project Settings.

#include "ETChatWindow.h"

#include "Package.h"
#include "Engine/World.h"

#include "Online/EveryThingGameState.h"
#include "Online/EveryThingPlayerState.h"

#include "Characters/PlayerPawnController.h"
#include "ChatWindow/ChatWindowTypes.h"
#include "ChatWindow/ChatComponent.h"
#include "ChatWindow/ChatWindowControllerInterface.h"
#include "ChatWindow/Channel/SystemChatChannel.h"
#include "ChatWindow/ChatWindowPlayerStateInterface.h"

#define LOCTEXT_NAMESPACE "EveryThing_GmaeUI_ETChatWindow"

void UETChatWindow::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerController* OwnerPC = GetOwningPlayer();
	UWorld* World = OwnerPC ? OwnerPC->GetWorld() : nullptr;

	APlayerPawnController* OwnerPPC = Cast<APlayerPawnController>(OwnerPC);
	if (OwnerPPC)
	{
		OwnerPPC->GetOnToggleToTargetRoleSuccessDelegate().AddUObject(this, &UETChatWindow::OnToggleToTargetRoleSuccess);
		OwnerPPC->GetOnToggleToTargetRoleFailureDelegate().AddUObject(this, &UETChatWindow::OnToggleToTargetRoleFailure);
	}
}


void UETChatWindow::OnToggleToTargetRoleSuccess(const FName& TargetRoleName)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("TargetRoleName"), FText::FromName(TargetRoleName));

	FText DisplayInfo = FText::Format(LOCTEXT("OnToggleToTargetRoleSuccess", "Success toggle toggle to target role : {TargetRoleName}."), Arguments);

	AddSystemMessage(ESystemMessageType::Success, DisplayInfo);
}


void UETChatWindow::OnToggleToTargetRoleFailure(const FName& TargetRoleName, const FText& ErrorInfo)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("TargetRoleName"), FText::FromName(TargetRoleName));
	Arguments.Add(TEXT("ErrorInfo"), ErrorInfo);


	FText DisplayInfo = FText::Format(LOCTEXT("OnToggleToTargetRoleFailure", "Failure toggle to target role : {TargetRoleName}, because: {ErrorInfo}"), Arguments);

	AddSystemMessage(ESystemMessageType::Error, DisplayInfo);
}

void UETChatWindow::OnPlayerJoinGame(APlayerState* InPlayerState)
{
	IChatWindowPlayerStateInterface* TargetCWPS = Cast<IChatWindowPlayerStateInterface>(InPlayerState);
	if (TargetCWPS)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("PlayerName"), FText::FromString(TargetCWPS->GetPlayerChatName()));
		FText DisplayInfo = FText::Format(LOCTEXT("OnPlayerJoinGame", "Welcome the Player {PlayerName}."), Arguments);

		AddSystemMessage(ESystemMessageType::Normal, DisplayInfo, false);
	}
}
void UETChatWindow::OnPlayerLeaveGame(APlayerState* InPlayerState)
{
	IChatWindowPlayerStateInterface* TargetCWPS = Cast<IChatWindowPlayerStateInterface>(InPlayerState);
	if (TargetCWPS)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("PlayerName"), FText::FromString(TargetCWPS->GetPlayerChatName()));
		FText DisplayInfo = FText::Format(LOCTEXT("OnPlayerLeaveGame", "The Player {PlayerName} have left the game"), Arguments);

		AddSystemMessage(ESystemMessageType::Normal, DisplayInfo, false);
	}
}


void UETChatWindow::AddSystemMessage(ESystemMessageType SystemMessageType, const FText& MessageText, bool bIsLocal)
{
	if (ChatComponent)
	{
		FSystemChatChannel* SystemChatChannel = static_cast<FSystemChatChannel*>(ChatComponent->GetChannelManager().GetChatchannelDefaultValueFromName("System"));
		if (SystemChatChannel)
		{
			SystemChatChannel->SetMessageType(SystemMessageType);
			FChatMessageInfo MessageInfo(MessageText.ToString(), SystemChatChannel->GetDisplayLineColor());
			if (bIsLocal)
			{
				ChatComponent->ReceiveChatMessage(SystemChatChannel, MessageInfo);
			}
			else
			{
				ChatComponent->CreateChatMessage(SystemChatChannel, MessageInfo);
			}
		}
	}
}


#undef LOCTEXT_NAMESPACE