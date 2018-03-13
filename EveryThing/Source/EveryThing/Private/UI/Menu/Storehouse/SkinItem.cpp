// Fill out your copyright notice in the Description page of Project Settings.

#include "SkinItem.h"

#include "EveryThingGameInstance.h"


void USkinItem::InitializeSkinItem_Implementation(const FName& InSkinName, int32 InCost, bool bInHaveGoods)
{
	InitializeGoodsItem(InCost, bInHaveGoods);

	SkinName = InSkinName;

	InitializeSkinItemDisplay(SkinName, bInHaveGoods);
}

void USkinItem::InitializeSkinItemDisplay_Implementation(const FName& InSkinName, bool bInHaveGoods)
{
	UpdateSkinItemDisplay(bInHaveGoods);
}

void USkinItem::OnBuyRoleItem()
{
	BuyGoodsItem();
}


bool USkinItem::BuyGoodsItem()
{
	if (IsHaveEnoughMoney())
	{
		UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
		if (OwnerETGI)
		{
			FPlayerInfo& PlayerInfo = OwnerETGI->GetPlayerInfo();
			PlayerInfo.AllHaveGamePawnSkinNames.AddUnique(SkinName);
			PlayerInfo.Gold -= GoodCost;

			OwnerETGI->UpdatePlayerInfo();

			UpdateSkinItemDisplay(true);

			return true;
		}
	}
	return false;
}
