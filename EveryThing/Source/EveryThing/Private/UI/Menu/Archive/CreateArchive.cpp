// Fill out your copyright notice in the Description page of Project Settings.

#include "CreateArchive.h"

#include "UI/Menu/EveryThingMenuHUD.h"
#include "EveryThingGameInstance.h"


void UCreateArchive::Cancel()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MainMenu); }
}

void UCreateArchive::CreateArchive(const FString& ArchiveName, const FString& PlayerName)
{
	UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerETGI && OwnerMenuHUD)
	{
		if (OwnerETGI->CreateArchive(ArchiveName, PlayerName))
		{
			OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MasterInterface);
		}
		else
		{
			OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::ErrorDialog);
			OwnerMenuHUD->SetErrorDialogMessage(TEXT("Can't Create Archive"));
		}

	}
}
