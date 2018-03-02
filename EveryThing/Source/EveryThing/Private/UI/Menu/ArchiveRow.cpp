// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchiveRow.h"

#include "UI/Menu/EveryThingMenuHUD.h"
#include "EveryThingGameInstance.h"

void UArchiveRow::InitializeArchiveRow_Implementation(const FString& InArchiveName)
{
	ArchiveName = InArchiveName;
}

void UArchiveRow::LoadArchive()
{

	UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerETGI && OwnerMenuHUD)
	{
		if (OwnerETGI->LoadArchiveFromName(ArchiveName))
		{
			OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::HouseMenu);
		}
		else
		{
			OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::ErrorDialog);
			OwnerMenuHUD->SetErrorDialogMessage(TEXT("Can't load Archive"));
		}

	}
}