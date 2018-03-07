// Fill out your copyright notice in the Description page of Project Settings.

#include "GoodItem.h"


#include "EveryThingGameInstance.h"


void UGoodsItem::InitializeGoodsItem(int32 InCost, bool bInHaveGoods)
{
	GoodCost = InCost;
	bHaveGoods = bInHaveGoods;
}

bool UGoodsItem::IsHaveEnoughMoney() const
{
	UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
	if (OwnerETGI)
	{
		return OwnerETGI->GetPlayerInfo().Gold >= GoodCost;
	}
	return false;
}

bool UGoodsItem::BuyGoodsItem()
{
	UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
	if (OwnerETGI)
	{
		FPlayerInfo& PlayerInfo = OwnerETGI->GetPlayerInfo();
		if (PlayerInfo.Gold >= GoodCost)
		{
			PlayerInfo.Gold -= GoodCost;
			
			OwnerETGI->SaveCurrentArchive();

			return true;
		}
	}
	return false;
}