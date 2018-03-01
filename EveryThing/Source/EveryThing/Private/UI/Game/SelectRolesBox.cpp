// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectRolesBox.h"

#include "Online/EveryThingPlayerState.h"

void USelectRolesBox::NativeConstruct()
{
	Super::NativeConstruct();

	AEveryThingPlayerState* OwnerPlaterState = GetOwningPlayer() ? Cast<AEveryThingPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
	if (OwnerPlaterState)
	{
		OwnerPlaterState->GetOnUpdatePlayerInfoDelegate().AddUObject(this, &USelectRolesBox::OnUpdatePlayerInfo);
		InitializesAllHaveRoles(OwnerPlaterState->GetPlayerInfo().AllHaveRolesName);
	}
}

void USelectRolesBox::OnUpdatePlayerInfo(const FPlayerInfo& InPlayerInfo)
{
	InitializesAllHaveRoles(InPlayerInfo.AllHaveRolesName);
}