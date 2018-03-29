// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingHUD_House.h"

#include "EveryThingAssetManager.h"
#include "UI/House/HouseLayout.h"

void AEveryThingHUD_House::BeginPlay()
{
	Super::BeginPlay();

	// try create HouseLayout widget and change input mode
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