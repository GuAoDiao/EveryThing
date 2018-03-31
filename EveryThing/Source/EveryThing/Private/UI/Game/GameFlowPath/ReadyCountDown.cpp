// Fill out your copyright notice in the Description page of Project Settings.

#include "ReadyCountDown.h"

#include "Online/EveryThingGameState_Game.h"

void UReadyCountDown::NativeConstruct()
{
	AEveryThingGameState_Game* OwnerETGS_G = GetWorld() ? GetWorld()->GetGameState<AEveryThingGameState_Game>() : nullptr;
	if (OwnerETGS_G)
	{
		UpdateReadyCountDownTime(OwnerETGS_G->GetRemaningReadyTime());
		OwnerETGS_G->OnRemaningReadyTimeUpdateDelegate.AddUObject(this, &UReadyCountDown::UpdateReadyCountDownTime);
	}

	Super::NativeConstruct();
}

