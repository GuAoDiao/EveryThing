// Fill out your copyright notice in the Description page of Project Settings.

#include "Storehouse.h"

#include "EveryThingAssetManager.h"
#include "EveryThingGameInstance.h"
#include "Characters/GamePawnManager.h"
#include "UI/Menu/EveryThingMenuHUD.h"
#include "UI/Menu/Storehouse/RoleItem.h"

void UStorehouse::NativeConstruct()
{
	UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
	if (OwnerETGI)
	{
		InitializeStorehouse(OwnerETGI->GetPlayerInfo());

		OwnerETGI->GetOnPlayerInfoUpdateDelegate().AddUObject(this, &UStorehouse::OnPlayerInfoUpdate);
	}
	
	Super::NativeConstruct();
}

void UStorehouse::InitializeStorehouse_Implementation(const FPlayerInfo& InPlayerInfo)
{
	InitializeStorehouseDisplay(InPlayerInfo);
}

void UStorehouse::InitializeStorehouseDisplay_Implementation(const FPlayerInfo& InPlayerInfo)
{
	TSubclassOf<UUserWidget> RoleItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("RoleItem");
	APlayerController* OnwerPC = GetOwningPlayer();

	if (RoleItemClass && OnwerPC)
	{
		const TMap<FName, FRoleInfo>& AllRolesInfo = UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetAllRolesInfo();
		for (TMap<FName, FRoleInfo>::TConstIterator It(AllRolesInfo); It; ++It)
		{
			const FRoleInfo& RoleInfo = It.Value();
			URoleItem* RoleItem = CreateWidget<URoleItem>(OnwerPC, RoleItemClass);
			if (RoleItem)
			{
				RoleItem->InitializeRoleItem(RoleInfo.Name, RoleInfo.Cost, InPlayerInfo.AllHaveRolesName.Contains(RoleInfo.Name));
				AddRoleItem(RoleItem);
			}
		}
	}

	OnPlayerInfoUpdate(InPlayerInfo);
}

void UStorehouse::OnPlayerInfoUpdate(const FPlayerInfo& InPlayerInfo)
{
	UpdateRoleItemListDisplay(InPlayerInfo.AllHaveRolesName);

	UpdatePlayerGoldDisplay(InPlayerInfo.Gold);
}

void UStorehouse::Backup()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MasterInterface); }
}