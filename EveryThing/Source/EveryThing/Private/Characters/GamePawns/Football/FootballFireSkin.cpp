// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballFireSkin.h"

FFootballFireSkin::FFootballFireSkin(UStaticMeshComponent* StaticMeshComp) : FGamePawnSkin(StaticMeshComp)
{
	MaterialNames.Add(TEXT("FootballBlack_Fire"));
	MaterialNames.Add(TEXT("FootballWhite_Fire"));
}