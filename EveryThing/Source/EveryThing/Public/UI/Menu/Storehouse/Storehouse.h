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
	
	UFUNCTION(BlueprintNativeEvent)
	void InitializeStorehouseDisplay(const FPlayerInfo& InPlayerInfo);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRoleItemListDisplay(const TArray<FName>& InAllHaveRolesName);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerGoldDisplay(int32 Gold);

	UFUNCTION(BlueprintImplementableEvent)
	void AddRoleItem(URoleItem* RoleItem);

	void OnPlayerInfoUpdate(const FPlayerInfo& InPlayerInfo);

	UFUNCTION(BlueprintCallable)
	void Backup();
};
