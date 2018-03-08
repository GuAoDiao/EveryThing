// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadingMap.h"

#include "EveryThingGameInstance.h"

void ULoadingMap::InitializeLoadingMap_Implementation(UEveryThingGameInstance* InETGY, const FString& MapName)
{
	OwnerETGY = InETGY;

	UpdateLoadingMapDisplay(MapName);
}
