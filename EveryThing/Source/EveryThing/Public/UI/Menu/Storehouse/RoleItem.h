// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Storehouse/GoodItem.h"
#include "RoleItem.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API URoleItem : public UGoodsItem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void InitializeRoleItem(const FName& InRoleName, int32 InCost, bool bInHaveGoods);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRoleItemDisplay(const FName& InRoleName, int32 InCost, bool bInHaveGoods);

	UFUNCTION(BlueprintCallable)
	void OnRoleItemClicked();
	
	virtual bool BuyGoodsItem() override;
private:
	FName RoleName;
};
