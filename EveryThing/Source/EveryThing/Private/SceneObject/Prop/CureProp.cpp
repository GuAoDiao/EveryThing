// Fill out your copyright notice in the Description page of Project Settings.

#include "CureProp.h"

#include "Characters/GamePawn.h"


bool UCureProp::BeUseByGamePawn(AGamePawn* InGamePawn)
{
	if (InGamePawn)
	{
		InGamePawn->ChangeDurability(200.f);
		return true;
	}
	return false;
}
