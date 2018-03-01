// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMenu.h"

#include "Engine/World.h"
#include "GameFramework/InputSettings.h"


#include "EveryThingGameInstance.h"
#include "UI/Game/EveryThingGameHUD.h"

UGameMenu::UGameMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UGameMenu::NativeConstruct()
{
	UInputSettings* InpueSettings = UInputSettings::GetInputSettings();
	if (InpueSettings)
	{
		TArray<FInputActionKeyMapping> EscapeActionMapping;
		InpueSettings->GetActionMappingByName("ToggleGameMenu", EscapeActionMapping);
		for (FInputActionKeyMapping& EscapeAction : EscapeActionMapping)
		{
			EscapeKeys.Add(EscapeAction.Key);
			
			UE_LOG(LogTemp, Log, TEXT("-_- current supprot key : %s"), *EscapeAction.Key.GetDisplayName().ToString());
		}
	}

	Super::NativeConstruct();
}

FReply UGameMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	UE_LOG(LogTemp, Log, TEXT("-_- current input key : %s"), *InKeyEvent.GetKey().GetDisplayName().ToString());

	if (EscapeKeys.Contains(InKeyEvent.GetKey()))
	{
		Backup();
		return FReply::Handled();
	}

	return NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UGameMenu::Confing()
{

}

void UGameMenu::Backup()
{
	AEveryThingGameHUD* OwnerETGH = GetOwningPlayer() ? Cast<AEveryThingGameHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerETGH) {OwnerETGH->RemoveGameMenu();}
}

void UGameMenu::ExitGame()
{
	UEveryThingGameInstance* OwnerGameInstance = GetWorld() ? GetWorld()->GetGameInstance<UEveryThingGameInstance>() : nullptr;
	if (OwnerGameInstance) { OwnerGameInstance->ExitGame(); }
}