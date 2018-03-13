// Fill out your copyright notice in the Description page of Project Settings.

#include "Storehouse.h"

#include "EveryThingAssetManager.h"
#include "EveryThingGameInstance.h"
#include "Characters/GamePawnManager.h"
#include "UI/Menu/EveryThingMenuHUD.h"
#include "UI/Menu/Storehouse/RoleItem.h"
#include "UI/Menu/Storehouse/SkinItem.h"
#include "UI/Menu/Storehouse/FormItem.h"
#include "Characters/Form/GamePawnFormClassInfo.h"
#include "Characters/Skin/GamePawnSkinClassInfo.h"

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
				RoleItem->InitializeRoleItem(RoleInfo.Name, RoleInfo.Cost, InPlayerInfo.AllHaveRoleNames.Contains(RoleInfo.Name));
				AddRoleItem(RoleItem);
			}
		}
	}

	TSubclassOf<UUserWidget> SkinItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("SkinItem");
	if (SkinItemClass)
	{
		const TMap<FName, FGamePawnSkinClassInfo*>& AllGamePawnSkinClassInfo = UGamePawnManager::GetAllGamePawnSkinClassInfo();
		for (TMap<FName, FGamePawnSkinClassInfo*>::TConstIterator It(AllGamePawnSkinClassInfo); It; ++It)
		{
			FGamePawnSkinClassInfo* SkinClassInfo = It.Value();
			if (SkinClassInfo)
			{
				USkinItem* SkinItem = CreateWidget<USkinItem>(OnwerPC, SkinItemClass);
				if (SkinItem)
				{
					SkinItem->InitializeSkinItem(SkinClassInfo->SkinName, SkinClassInfo->Cost, InPlayerInfo.AllHaveGamePawnSkinNames.Contains(SkinClassInfo->SkinName));
					AddSkinItem(SkinItem);
				}
			}
		}
	}


	TSubclassOf<UUserWidget> FormItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("FormItem");
	if (FormItemClass)
	{
		const TMap<FName, FGamePawnFormClassInfo*>& AllGamePawnFormClassInfo = UGamePawnManager::GetAllGamePawnFormClassInfo();
		for (TMap<FName, FGamePawnFormClassInfo*>::TConstIterator It(AllGamePawnFormClassInfo); It; ++It)
		{
			FGamePawnFormClassInfo* FormClassInfo = It.Value();
			if (FormClassInfo)
			{
				UFormItem* FormItem = CreateWidget<UFormItem>(OnwerPC, FormItemClass);
				if (FormItem)
				{
					FormItem->InitializeFormItem(FormClassInfo->FormName, FormClassInfo->Cost, InPlayerInfo.AllHaveGamePawnFormNames.Contains(FormClassInfo->FormName));
					AddFormItem(FormItem);
				}
			}
		}
	}

	OnPlayerInfoUpdate(InPlayerInfo);
}

void UStorehouse::OnPlayerInfoUpdate(const FPlayerInfo& InPlayerInfo)
{
	UpdateStoreHoustDisplay(InPlayerInfo);
}

void UStorehouse::Backup()
{
	AEveryThingMenuHUD* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingMenuHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MasterInterface); }
}