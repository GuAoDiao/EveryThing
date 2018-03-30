// Fill out your copyright notice in the Description page of Project Settings.

#include "UserInfoBox.h"

#include "Online/EveryThingPlayerState_Game.h"
#include "Online/PlayerController_Game.h"
#include "Characters/GamePawn.h"

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
		OnPlayerInfoUpdate(OwnerETPS_G->GetPlayerInfo());
		OnPlayerTeamUpdate(OwnerETPS_G->GetTeamID());
		OwnerETPS_G->OnTeamIDUpdateDelegate.AddUObject(this, &UUserInfoBox::OnPlayerTeamUpdate);
		OwnerETPS_G->OnPlayerInfoUpdateDelegate.AddUObject(this, &UUserInfoBox::OnPlayerInfoUpdate);
	}
}

void UUserInfoBox::OnRoleNameUpdate(const FName& RoleName)
{
	InitializeCurrentRoleName(RoleName);

	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	AGamePawn* OwnerGamePawn = OwnerPC_G ? Cast<AGamePawn>(OwnerPC_G->GetPawn()) : nullptr;
	if (OwnerGamePawn)
	{
		Durability = OwnerGamePawn->GetDurability();
		Stamina = OwnerGamePawn->GetStamina();
		MaxDurability = OwnerGamePawn->GetMaxDurability();
		MaxStamina = OwnerGamePawn->GetMaxStamina();

		UpdateDurability(Durability, MaxDurability);
		UpdateStamina(Stamina, MaxStamina);

		OwnerGamePawn->OnMaxDurabilityUpdateDelegate.AddUObject(this, &UUserInfoBox::OnMaxDurabilityUpdate);
		OwnerGamePawn->OnMaxStaminaUpdateDelegate.AddUObject(this, &UUserInfoBox::OnMaxStaminaUpdate);
		OwnerGamePawn->OnDurabilityUpdateDelegate.AddUObject(this, &UUserInfoBox::OnDurabilityUpdate);
		OwnerGamePawn->OnStaminaUpdateDelegate.AddUObject(this, &UUserInfoBox::OnStaminaUpdate);
	}
}

void UUserInfoBox::OnPlayerTeamUpdate(int32 TeamID)
{
	if (TeamID > 0)
	{
		UpdateTeamDisplay(true, TeamID);
	}
	else
	{
		UpdateTeamDisplay(false, -1);
	}
}