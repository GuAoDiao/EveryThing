// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectRolesBox.h"

#include "Online/EveryThingPlayerState.h"

void USelectRolesBox::NativeConstruct()
{
	Super::NativeConstruct();

	AEveryThingPlayerState* OwnerPlaterState = GetOwningPlayer() ? Cast<AEveryThingPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
	if (OwnerPlaterState)
	{
		InitializesAllHaveRoles(OwnerPlaterState->GetPlayerInfo().AllHaveRolesName);
	}
}