// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GamePawnManager.h"
#include "Characters/Skin/GamePawnSkinClassInfo.h"

class UStaticMeshComponent;


#define DECLARE_GAMEPAWNSKIN_CLASS(SkinName) \
protected: \
	const static FGamePawnSkinClassInfo SkinClassInfo; \
public: \
	virtual const FGamePawnSkinClassInfo* GetGamePawnSkinClassInfo() const; \
	static FGamePawnSkin* CreateGamePawnSkin(class UStaticMeshComponent* InStaticMeshComp);

#define IMPLEMENT_GAMEPAWNSKIN_CLASS(SkinName, SkinClass) \
	const FGamePawnSkinClassInfo SkinClass::SkinClassInfo = FGamePawnSkinClassInfo(SkinName, &SkinClass::CreateGamePawnSkin); \
	const FGamePawnSkinClassInfo* SkinClass::GetGamePawnSkinClassInfo() const {return &SkinClass::SkinClassInfo; } \
	FGamePawnSkin* SkinClass::CreateGamePawnSkin(class UStaticMeshComponent* InStaticMeshComp) {return new SkinClass(InStaticMeshComp);}

class EVERYTHING_API FGamePawnSkin
{	
public:
	FGamePawnSkin(UStaticMeshComponent* StaticMeshComp);
	virtual ~FGamePawnSkin() {}
	
	DECLARE_GAMEPAWNSKIN_CLASS("GamePawnSkin");

	void LoadAllGameSkinMaterial();

	virtual void LoadGamePawnSkin();

	void UnloadGamePawnSkin();

protected:
	UStaticMeshComponent* OwnerStaticMeshComp;

	TArray<FName> MaterialNames;
};
