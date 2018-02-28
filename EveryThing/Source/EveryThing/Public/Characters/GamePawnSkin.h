// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UStaticMeshComponent;

class EVERYTHING_API FGamePawnSkin
{	
public:
	FGamePawnSkin(UStaticMeshComponent* StaticMeshComp);
	virtual ~FGamePawnSkin() {}


	void LoadAllGameSkinMaterial();

	virtual void LoadGamePawnSkin();

	void UnloadGamePawnSkin();

protected:
	UStaticMeshComponent* OwnerStaticMeshComp;

	TArray<FName> MaterialNames;
};
