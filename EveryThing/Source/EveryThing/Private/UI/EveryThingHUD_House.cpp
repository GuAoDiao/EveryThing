// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingHUD_House.h"

#include "EveryThingAssetManager.h"
#include "UI/Menu/House/HouseLayout.h"

void AEveryThingHUD_House::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* OwnerPC = GetOwningPlayerController();
	TSubclassOf<UUserWidget> HouseLayoutClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("HouseLayout");
	if (OwnerPC && HouseLayoutClass)
	{
		HouseLayout = CreateWidget<UHouseLayout>(OwnerPC, HouseLayoutClass);
		if (HouseLayout)
		{
			HouseLayout->AddToViewport();

			OwnerPC->bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			InputMode.SetWidgetToFocus(HouseLayout->TakeWidget());
			OwnerPC->SetInputMode(InputMode);
		}
	}
}

void AEveryThingHUD_House::BeginDestroy()
{
	if (HouseLayout && HouseLayout->IsInViewport())
	{
		HouseLayout->RemoveFromViewport();
		HouseLayout = nullptr;
	}

	Super::BeginDestroy();
}
