// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseCreate.h"


#include "EveryThingGameInstance.h"
#include "UI/EveryThingMenuHUD.h"

UHouseCreate::UHouseCreate(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UHouseCreate::HostGame(const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum)
{
	UWorld* World = GetWorld();
	UEveryThingGameInstance* OwnerGameInstance = World ? World->GetGameInstance<UEveryThingGameInstance>() : nullptr;
	if (OwnerGameInstance) { OwnerGameInstance->HostGame(GameType, MapName, bIsLAN, bIsPresence, MaxPlayersNum); }
}


void UHouseCreate::Back()
{
	APlayerController* OwnerPC = GetOwningPlayer();
	AEveryThingMenuHUD* OwnerMenuHUD = OwnerPC ? Cast<AEveryThingMenuHUD>(OwnerPC->GetHUD()) : nullptr;
	if (OwnerMenuHUD)
	{
		OwnerMenuHUD->ToggleToNewGameUIState(EGameUIState::MainMenu);
	}
}