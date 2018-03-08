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
	if (OwnerETGI) { InitializeStorehouse(OwnerETGI->GetPlayerInfo()); }

	Super::NativeConstruct();
}

void UStorehouse::InitializeStorehouse_Implementation(const FPlayerInfo& InPlayerInfo)
{
	UpdateStorehouseDisplay(InPlayerInfo);

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
}


void UStorehouse::Backup()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MasterInterface); }
}