// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EveryThingTypes.h"

#include "SelectRolesBox.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API USelectRolesBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitializesAllHaveRoles(const TArray<FName>& AllHaveRolesName);

	void OnUpdatePlayerInfo(const FPlayerInfo& InPlayerInfo);
};
