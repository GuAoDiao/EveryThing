// Fill out your copyright notice in the Description page of Project Settings.

#include "ErrorDialog.h"


#include "EveryThingGameInstance.h"
#include "UI/Menu/EveryThingMenuHUD.h"

void UErrorDialog::Close()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToLastGameUIState(); }
}