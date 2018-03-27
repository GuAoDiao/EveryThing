// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectSkinsBox.h"

#include "Online/EveryThingPlayerState.h"
#include "Online/PlayerController_Game.h"
#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"
#include "SelectItem.h"
#include "Characters/GamePawn.h"

void USelectSkinsBox::NativeConstruct()
{
	Super::NativeConstruct();

	AEveryThingPlayerState* OwnerPlaterState = GetOwningPlayer() ? Cast<AEveryThingPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
	if (OwnerPlaterState) { OwnerPlaterState->OnUpdatePlayerInfoDelegate.AddUObject(this, &USelectSkinsBox::OnUpdatePlayerInfo); }

	APlayerController_Game* OwnerPPC = Cast<APlayerController_Game>(GetOwningPlayer());
	if (OwnerPPC)
	{
		OwnerPPC->OnToggleToTargetRoleSuccessDelegate.AddUObject(this, &USelectSkinsBox::InitializeSelectSkinsBoxDisplay);
		InitializeSelectSkinsBoxDisplay(OwnerPPC->GetCurrentRoleName());
	}
}

void USelectSkinsBox::InitializeSelectSkinsBoxDisplay_Implementation(const FName& TargetRoleName)
{
	AEveryThingPlayerState* OwnerPlaterState = GetOwningPlayer() ? Cast<AEveryThingPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
	TSubclassOf<UUserWidget> SelectItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("SelectItem");
	APlayerController_Game* OwnerPPC = Cast<APlayerController_Game>(GetOwningPlayer());

	if (SelectItemClass && OwnerPPC && OwnerPlaterState)
	{
		const FPlayerInfo& PlayerInfo = OwnerPlaterState->GetPlayerInfo();
		for (const FName& SkinName : UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetAllRoleSkinWithRoleName(TargetRoleName))
		{
			USelectItem* SelectItem = CreateWidget<USelectItem>(OwnerPPC, SelectItemClass);
			if (SelectItem)
			{
				SelectItem->InitializeSelectItem(SkinName, PlayerInfo.AllHaveGamePawnSkinNames.Contains(SkinName));
				AddSelectItem(SelectItem);
			}
		}
	}
}

void USelectSkinsBox::OnUpdatePlayerInfo(const FPlayerInfo& InPlayerInfo)
{
	UpdateSelectSkinsBoxDisplay(InPlayerInfo.AllHaveGamePawnSkinNames);
}

