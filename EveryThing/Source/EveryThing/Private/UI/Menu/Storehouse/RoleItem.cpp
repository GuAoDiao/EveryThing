// Fill out your copyright notice in the Description page of Project Settings.

#include "RoleItem.h"

#include "EveryThingGameInstance.h"


void URoleItem::InitializeRoleItem_Implementation(const FName& InRoleName, int32 InCost, bool bInHaveGoods)
{
	InitializeGoodsItem(InCost, bInHaveGoods);

	RoleName = InRoleName;

	UpdateRoleItemDisplay(RoleName, InCost, bInHaveGoods);
}

void URoleItem::OnRoleItemClicked()
{
	if (HaveGoods())
	{

	}
	else
	{
		BuyGoodsItem();
	}
}


bool URoleItem::BuyGoodsItem()
{
	if (Super::BuyGoodsItem())
	{
		UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
		if (OwnerETGI)
		{
			FPlayerInfo& PlayerInfo = OwnerETGI->GetPlayerInfo();
			PlayerInfo.AllHaveRolesName.Add(RoleName);

			OwnerETGI->SaveCurrentArchive();
			return true;
		}
	}
	return false;
}
