// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectFormsBox.h"

#include "Online/EveryThingPlayerState.h"
#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"
#include "SelectItem.h"
#include "Online/PlayerController_Game.h"
#include "Characters/GamePawn.h"

void USelectFormsBox::NativeConstruct()
{
	Super::NativeConstruct();

	AEveryThingPlayerState* OwnerPlaterState = GetOwningPlayer() ? Cast<AEveryThingPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
	if (OwnerPlaterState) { OwnerPlaterState->OnUpdatePlayerInfoDelegate.AddUObject(this, &USelectFormsBox::OnUpdatePlayerInfo); }

	APlayerController_Game* OwnerPPC = Cast<APlayerController_Game>(GetOwningPlayer());
	if (OwnerPPC)
	{
		OwnerPPC->OnToggleToTargetRoleSuccessDelegate.AddUObject(this, &USelectFormsBox::InitializeSelectFormsBoxDisplay);
		InitializeSelectFormsBoxDisplay(OwnerPPC->GetCurrentRoleName());
	}
}

void USelectFormsBox::InitializeSelectFormsBoxDisplay_Implementation(const FName& TargetRoleName)
{
	AEveryThingPlayerState* OwnerPlaterState = GetOwningPlayer() ? Cast<AEveryThingPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
	TSubclassOf<UUserWidget> SelectItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("SelectItem");
	APlayerController_Game* OwnerPPC = Cast<APlayerController_Game>(GetOwningPlayer());

	if (SelectItemClass && OwnerPPC && OwnerPlaterState)
	{
		const FPlayerInfo& PlayerInfo = OwnerPlaterState->GetPlayerInfo();
		for (const FName& FormName : UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetAllRoleFormWithRoleName(TargetRoleName))
		{
			USelectItem* SelectItem = CreateWidget<USelectItem>(OwnerPPC, SelectItemClass);
			if (SelectItem)
			{
				SelectItem->InitializeSelectItem(FormName, PlayerInfo.AllHaveGamePawnFormNames.Contains(FormName));
				AddSelectItem(SelectItem);
			}
		}
	}
}

void USelectFormsBox::OnUpdatePlayerInfo(const FPlayerInfo& InPlayerInfo)
{
	UpdateSelectFormsBoxDisplay(InPlayerInfo.AllHaveGamePawnFormNames);
}