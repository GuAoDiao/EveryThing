// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Storehouse/GoodItem.h"
#include "SkinItem.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API USkinItem : public UGoodsItem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void InitializeSkinItem(const FName& InSkinName, int32 InCost, bool bInHaveGoods);
	
	UFUNCTION(BlueprintNativeEvent)
	void InitializeSkinItemDisplay(const FName& InSkinName, bool bInHaveGoods);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateSkinItemDisplay(bool bInHaveGoods);

	UFUNCTION(BlueprintPure)
	const FName& GetSkinName() { return SkinName; }

	UFUNCTION(BlueprintCallable)
	void OnBuyRoleItem();
	
	virtual bool BuyGoodsItem() override;
private:
	FName SkinName;
	
	
};
