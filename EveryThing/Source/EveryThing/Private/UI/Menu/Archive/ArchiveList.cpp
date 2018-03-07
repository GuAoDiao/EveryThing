// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchiveList.h"

#include "UI/Menu/EveryThingMenuHUD.h"
#include "EveryThingGameInstance.h"


void UArchiveList::NativeConstruct()
{
	UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
	if (OwnerETGI && OwnerETGI->LoadArchivesList())
	{
		ShowArchiveList(OwnerETGI->GetArchivesList());
	}
}

void UArchiveList::Backup()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MainMenu); }
}