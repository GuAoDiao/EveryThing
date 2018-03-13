// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectFormsBox.h"

#include "Online/EveryThingPlayerState.h"

void USelectFormsBox::NativeConstruct()
{
	Super::NativeConstruct();

	AEveryThingPlayerState* OwnerPlaterState = GetOwningPlayer() ? Cast<AEveryThingPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
	if (OwnerPlaterState)
	{
		OwnerPlaterState->GetOnUpdatePlayerInfoDelegate().AddUObject(this, &USelectFormsBox::OnUpdatePlayerInfo);
		UpdateSelectFormsBoxDisplay(OwnerPlaterState->GetPlayerInfo().AllHaveGamePawnFormNames);
	}
}

void USelectFormsBox::OnUpdatePlayerInfo(const FPlayerInfo& InPlayerInfo)
{
	UpdateSelectFormsBoxDisplay(InPlayerInfo.AllHaveGamePawnFormNames);
}