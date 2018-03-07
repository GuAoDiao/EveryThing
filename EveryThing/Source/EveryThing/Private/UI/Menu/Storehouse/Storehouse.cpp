// Fill out your copyright notice in the Description page of Project Settings.

#include "Storehouse.h"

#include "EveryThingAssetManager.h"
#include "EveryThingGameInstance.h"
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
	UDataTable* RoleNameDatatable = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("RolesInfo"));
	APlayerController* OnwerPC = GetOwningPlayer();

	if (RoleNameDatatable && RoleItemClass && OnwerPC)
	{
		TArray<FRoleInfo*> RolesNameDataInDatatable;
		RoleNameDatatable->GetAllRows<FRoleInfo>(TEXT("found all Roles Name in DataTable"), RolesNameDataInDatatable);
		
		for (FRoleInfo* RoleNameData : RolesNameDataInDatatable)
		{
			URoleItem* RoleItem = CreateWidget<URoleItem>(OnwerPC, RoleItemClass);
			if (RoleItem)
			{
				RoleItem->InitializeRoleItem(RoleNameData->Name, RoleNameData->Cost, InPlayerInfo.AllHaveRolesName.Contains(RoleNameData->Name));
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