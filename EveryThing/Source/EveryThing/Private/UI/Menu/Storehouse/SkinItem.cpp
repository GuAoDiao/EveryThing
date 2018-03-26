// Fill out your copyright notice in the Description page of Project Settings.

#include "SkinItem.h"

#include "EveryThingGameInstance.h"
#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"
#include "UI/Menu/Storehouse/Storehouse.h"


void USkinItem::InitializeSkinItem(UStorehouse* StoreHouse, const FName& InSkinName, bool bInHaveGoods)
{
	SkinName = InSkinName;
	OwnerStoreHouse = StoreHouse;

	UGamePawnManager* GamePawnManager = UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager();
	if (GamePawnManager->GetRoleSkinInfoFromName(InSkinName, SkinInfo) && SkinInfo)
	{
		InitializeGoodsItem(SkinInfo->Cost, bInHaveGoods);

		InitializeSkinItemDisplay();

		UpdateIsHaveGoods(bInHaveGoods);
	}
}

void USkinItem::OnBuySkinItem(){BuyGoodsItem();}
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

			UpdateIsHaveGoods(true);

			return true;
		}
	}
	return false;
}

void USkinItem::OnDisplaySkinItem() { if (OwnerStoreHouse) { OwnerStoreHouse->ToggleDisplaySkin(SkinName); } }
