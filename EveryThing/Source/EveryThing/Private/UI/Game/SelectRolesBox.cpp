// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectRolesBox.h"

#include "Online/EveryThingPlayerState.h"
#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"
#include "UI/Game/SelectItem.h"

void USelectRolesBox::NativeConstruct()
{
	Super::NativeConstruct();

	AEveryThingPlayerState* OwnerPlaterState = GetOwningPlayer() ? Cast<AEveryThingPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
	if (OwnerPlaterState) { OwnerPlaterState->GetOnUpdatePlayerInfoDelegate().AddUObject(this, &USelectRolesBox::OnUpdatePlayerInfo); }

	TSubclassOf<UUserWidget> SelectItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("SelectItem");
	APlayerController* OnwerPC = GetOwningPlayer();
	if (SelectItemClass && OnwerPC && OwnerPlaterState)
	{
		const FPlayerInfo& PlayerInfo = OwnerPlaterState->GetPlayerInfo();
		const TMap<FName, FRoleInfo>& AllRolesInfo = UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetAllRolesInfo();
		for (TMap<FName, FRoleInfo>::TConstIterator It(AllRolesInfo); It; ++It)
		{
			const FRoleInfo& RoleInfo = It.Value();
			USelectItem* SelectItem = CreateWidget<USelectItem>(OnwerPC, SelectItemClass);
			if (SelectItem)
			{
				SelectItem->InitializeSelectItem(RoleInfo.Name, PlayerInfo.AllHaveRoleNames.Contains(RoleInfo.Name));
				AddSelectItem(SelectItem);
			}
		}
	}
}

void USelectRolesBox::OnUpdatePlayerInfo(const FPlayerInfo& InPlayerInfo)
{
	UpdateSelectRolesBoxDisplay(InPlayerInfo.AllHaveRoleNames);
}