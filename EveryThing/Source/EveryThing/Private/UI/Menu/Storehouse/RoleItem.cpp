// Fill out your copyright notice in the Description page of Project Settings.

#include "RoleItem.h"

#include "EveryThingGameInstance.h"


void URoleItem::InitializeRoleItem_Implementation(const FName& InRoleName, int32 InCost, bool bInHaveGoods)
{
	InitializeGoodsItem(InCost, bInHaveGoods);

	RoleName = InRoleName;

	InitializeRoleItemDisplay(RoleName, bInHaveGoods);
}

void URoleItem::InitializeRoleItemDisplay_Implementation(const FName& InRoleName, bool bInHaveGoods)
{
	UpdateRoleItemDisplay(bInHaveGoods);
}

void URoleItem::OnBuyRoleItem()
{
	BuyGoodsItem();
}


bool URoleItem::BuyGoodsItem()
{
	if (IsHaveEnoughMoney())
	{
		UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
		if (OwnerETGI)
		{
			FPlayerInfo& PlayerInfo = OwnerETGI->GetPlayerInfo();
			PlayerInfo.AllHaveRolesName.Add(RoleName);
			PlayerInfo.Gold -= GoodCost;

			OwnerETGI->UpdatePlayerInfo();

			UpdateRoleItemDisplay(true);

			return true;
		}
	}
	return false;
}
