// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Form/GamePawnFormClassInfo.h"

class AGamePawn;
class USkillComponent;
class UAttackComponent;

#define DECLARE_GAMEPAWNFORM_CLASS(FormName) \
protected: \
	const static FGamePawnFormClassInfo FormClassInfo; \
public: \
	virtual const FGamePawnFormClassInfo* GetGamePawnSkinClassInfo() const; \
	static FGamePawnForm* CreateGamePawnForm(class AGamePawn* InGamePawn);

#define IMPLEMENT_GAMEPAWNFORM_CLASS(FormName, FormClass) \
	const FGamePawnFormClassInfo FormClass::FormClassInfo = FGamePawnFormClassInfo(FormName, &FormClass::CreateGamePawnForm); \
	const FGamePawnFormClassInfo* FormClass::GetGamePawnSkinClassInfo() const {return &FormClass::FormClassInfo; } \
	FGamePawnForm* FormClass::CreateGamePawnForm(class AGamePawn* InGamePawn) {return new FormClass(InGamePawn);}


class EVERYTHING_API FGamePawnForm
{	
public:
	FGamePawnForm(AGamePawn* InGamePawn);

	virtual void LoadGamePawnForm();
	void UnloadGamePawnForm();
protected:
	AGamePawn* OwnerGamePawn;

	UClass* SkillCompClass;
	UClass* AttackCompClass;
};
