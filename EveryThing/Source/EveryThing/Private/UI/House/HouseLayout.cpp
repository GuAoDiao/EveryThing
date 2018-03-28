// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseLayout.h"

#include "EveryThingAssetManager.h"
#include "UI/House/HousePlayerItem.h"
#include "Online/EveryThingGameState_House.h"
#include "Online/EveryThingPlayerState_House.h"
#include "Online/PlayerController_House.h"

void UHouseLayout::NativeConstruct()
{
	Super::NativeConstruct();

	AEveryThingGameState_House* OwnerETGS_H = GetWorld() ? GetWorld()->GetGameState<AEveryThingGameState_House>() : nullptr;
	if (OwnerETGS_H)
	{
		for (APlayerState* PlayerState : OwnerETGS_H->PlayerArray)
		{
			OnAddPlayer(PlayerState);
		}

		OwnerETGS_H->OnAddPlayerDelegate.AddUObject(this, &UHouseLayout::OnAddPlayer);
	}

	APlayerController_House* OwnerPC_H = Cast<APlayerController_House>(GetOwningPlayer());
	if (OwnerPC_H)
	{
		OnPlayerStateUpdate(OwnerPC_H->PlayerState);
		OwnerPC_H->OnPlayerStateUpdateDelegate.AddUObject(this, &UHouseLayout::OnPlayerStateUpdate);
	}
}

void UHouseLayout::OnPlayerStateUpdate(class APlayerState* PlayerState)
{
	AEveryThingPlayerState_House* OwnerETPS_H = Cast<AEveryThingPlayerState_House>(PlayerState);
	if (OwnerETPS_H)
	{
		UpdateIsHouseOwner(OwnerETPS_H->CheckIsHouseOwner());
		UpdateReadyState(OwnerETPS_H->GetIsReady());
		OwnerETPS_H->OnbIsReadyUpdateDelegate.AddUObject(this, &UHouseLayout::UpdateReadyState);
	}
}

void UHouseLayout::OnAddPlayer(class APlayerState* PlayerState)
{
	APlayerController* OwnerPC = GetOwningPlayer();
	TSubclassOf<UUserWidget> HousePlayerItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("HousePlayerItem");
	AEveryThingPlayerState_House* NewETPS = Cast<AEveryThingPlayerState_House>(PlayerState);
	if (OwnerPC && HousePlayerItemClass && NewETPS)
	{
		UHousePlayerItem* HousePlayerItem = CreateWidget<UHousePlayerItem>(OwnerPC, HousePlayerItemClass);
		if (HousePlayerItem)
		{
			HousePlayerItem->InitializeHousePlayerItem(NewETPS);
			AddHousePlayerItem(HousePlayerItem);

			AllPlayerItemList.Add(PlayerState, HousePlayerItem);
		}
	}
}

void UHouseLayout::OnRemovePlayer(class APlayerState* PlayerState)
{
	if (AllPlayerItemList.Contains(PlayerState))
	{
		RemoveHousePlayerItem(AllPlayerItemList[PlayerState]);

		AllPlayerItemList.Remove(PlayerState);
	}
}



void UHouseLayout::ToggleReadState()
{
	APlayerController* OwnerPC = GetOwningPlayer();
	AEveryThingPlayerState_House* OwnerETPS_H = OwnerPC ? Cast<AEveryThingPlayerState_House>(OwnerPC->PlayerState) : nullptr;
	if (OwnerETPS_H)
	{
		OwnerETPS_H->TooggleReadState();
		UpdateReadyState(OwnerETPS_H->GetIsReady());
	}
}

void UHouseLayout::StartGame()
{
	APlayerController_House* OwnerPC_H = Cast<APlayerController_House>(GetOwningPlayer());
	if (OwnerPC_H) { OwnerPC_H->StartGameWhenIsHouseOwner(); }
}