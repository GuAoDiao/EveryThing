// Fill out your copyright notice in the Description page of Project Settings.

#include "DirectlyCurePropItem.h"

#include "Characters/GamePawn.h"

#define LOCTEXT_NAMESPACE "Everything_SceneObject_DirectlyCurePropItem"


void ADirectlyCurePropItem::BeOverlapByGamePawn(class AGamePawn* OverlapGamePawn)
{
	if (OverlapGamePawn)
	{
		OverlapGamePawn->ChangeDurability(200.f);
		CreatePlayerFightInfoFromProp(OverlapGamePawn, LOCTEXT("OnBeCure", "You've recovered 200 Durability."));
	}
}

#undef LOCTEXT_NAMESPACE