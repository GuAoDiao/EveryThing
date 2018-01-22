// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnSkin.h"

#include "Components/StaticMeshComponent.h"

FGamePawnSkin::FGamePawnSkin(UStaticMeshComponent* StaticMeshComp)
{
	OwnerStaticMeshComp = StaticMeshComp;
}

void FGamePawnSkin::UnloadGamePawnSkin()
{

}