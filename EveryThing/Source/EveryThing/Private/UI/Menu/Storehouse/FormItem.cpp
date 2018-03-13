// Fill out your copyright notice in the Description page of Project Settings.

#include "FormItem.h"


#include "EveryThingGameInstance.h"


void UFormItem::InitializeFormItem_Implementation(const FName& InFormName, int32 InCost, bool bInHaveGoods)
{
	InitializeGoodsItem(InCost, bInHaveGoods);

	FormName = InFormName;

	InitializeFormItemDisplay(FormName, bInHaveGoods);
}

void UFormItem::InitializeFormItemDisplay_Implementation(const FName& InFormName, bool bInHaveGoods)
{
	UpdateFormItemDisplay(bInHaveGoods);
}

void UFormItem::OnBuyFormItem()
{
	BuyGoodsItem();
}


bool UFormItem::BuyGoodsItem()
{
	if (IsHaveEnoughMoney())
	{
		UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
		if (OwnerETGI)
		{
			FPlayerInfo& PlayerInfo = OwnerETGI->GetPlayerInfo();
			PlayerInfo.AllHaveGamePawnFormNames.AddUnique(FormName);
			PlayerInfo.Gold -= GoodCost;

			OwnerETGI->UpdatePlayerInfo();

			UpdateFormItemDisplay(true);

			return true;
		}
	}
	return false;
}



