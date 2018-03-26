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
	void InitializeRoleItem(class UStorehouse* StoreHouse, const FName& InRoleName, int32 InCost, bool bInHaveGoods);
	
	UFUNCTION(BlueprintNativeEvent)
	void InitializeRoleItemDisplay(const FName& InRoleName, bool bInHaveGoods);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateRoleItemDisplay(bool bInHaveGoods);

	UFUNCTION(BlueprintPure)
	const FName& GetRoleName() { return RoleName; }

	UFUNCTION(BlueprintCallable)
	void OnBuyRoleItem();
	
	UFUNCTION(BlueprintCallable)
	void OnDisplayRoleItem();

	virtual bool BuyGoodsItem() override;
private:
	FName RoleName;
	UStorehouse* OwnerStoreHouse;
};
