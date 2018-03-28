// Fill out your copyright notice in the Description page of Project Settings.

#include "UserInfoBox.h"

#include "Online/EveryThingPlayerState_Game.h"
#include "Online/PlayerController_Game.h"

void UUserInfoBox::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	if (OwnerPC_G)
	{
		InitializeCurrentRoleName(OwnerPC_G->GetCurrentRoleName());
		OwnerPC_G->OnRoleNameUpdateDelegate.AddUObject(this, &UUserInfoBox::OnRoleNameUpdate);

		OnPlayerStateUpdate(OwnerPC_G->PlayerState);
		OwnerPC_G->OnPlayerStateUpdateDelegate.AddUObject(this, &UUserInfoBox::OnPlayerStateUpdate);
	}
}

void UUserInfoBox::OnPlayerStateUpdate(class APlayerState* PlayerState)
{
	AEveryThingPlayerState_Game* OwnerETPS_G = Cast<AEveryThingPlayerState_Game>(PlayerState);
	if (OwnerETPS_G)
	{
		OnUpdatePlayerInfo(OwnerETPS_G->GetPlayerInfo());
		OwnerETPS_G->OnUpdatePlayerInfoDelegate.AddUObject(this, &UUserInfoBox::OnUpdatePlayerInfo);
	}
}