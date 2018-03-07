// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EveryThingTypes.h"

#include "Storehouse.generated.h"

class URoleItem;

/**
 * 
 */
UCLASS()
class EVERYTHING_API UStorehouse : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent)
	void InitializeStorehouse(const FPlayerInfo& InPlayerInfo);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStorehouseDisplay(const FPlayerInfo& InPlayerInfo);

	UFUNCTION(BlueprintImplementableEvent)
	void AddRoleItem(URoleItem* RoleItem);

	UFUNCTION(BlueprintCallable)
	void Backup();
};
