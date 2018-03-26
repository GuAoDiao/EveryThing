// Fill out your copyright notice in the Description page of Project Settings.

#include "RoleItem.h"

#include "EveryThingGameInstance.h"
#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"
#include "UI/Menu/Storehouse/Storehouse.h"


void URoleItem::InitializeRoleItem_Implementation(UStorehouse* StoreHouse, const FName& InRoleName, int32 InCost, bool bInHaveGoods)
{
	InitializeGoodsItem(InCost, bInHaveGoods);

	OwnerStoreHouse = StoreHouse;
	RoleName = InRoleName;

	InitializeRoleItemDisplay(RoleName, bInHaveGoods);
}

void URoleItem::InitializeRoleItemDisplay_Implementation(const FName& InRoleName, bool bInHaveGoods) { UpdateRoleItemDisplay(bInHaveGoods); }

void URoleItem::OnBuyRoleItem() { BuyGoodsItem(); }
bool URoleItem::BuyGoodsItem()
{
	if (IsHaveEnoughMoney())
	{
		UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
		if (OwnerETGI)
		{
			FPlayerInfo& PlayerInfo = OwnerETGI->GetPlayerInfo();
			PlayerInfo.AllHaveRoleNames.AddUnique(RoleName);
			
			const FRoleInfo* RoleInfo;
			if (UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetRoleInfoFromName(RoleName, RoleInfo))
			{
				PlayerInfo.AllHaveGamePawnSkinNames.AddUnique(RoleInfo->DefaultSkinName);
				PlayerInfo.AllHaveGamePawnFormNames.AddUnique(RoleInfo->DefaultFormName);
			}

			PlayerInfo.Gold -= GoodCost;
			
			OwnerETGI->UpdatePlayerInfo();

			UpdateRoleItemDisplay(true);

			return true;
		}
	}
	return false;
}

void URoleItem::OnDisplayRoleItem() { if (OwnerStoreHouse) { OwnerStoreHouse->ToggleDisplayRole(RoleName); } }
