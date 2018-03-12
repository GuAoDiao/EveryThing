// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoodItem.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class EVERYTHING_API UGoodsItem : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	void InitializeGoodsItem(int32 InCost, bool bInHaveGoods);
	
	bool IsHaveEnoughMoney() const;

	virtual bool BuyGoodsItem() { return false; }
	UFUNCTION(BlueprintPure)
	int32 GetGoodsCost() const { return GoodCost; }
	bool HaveGoods() const { return bHaveGoods; }
protected:
	int32 GoodCost;
	bool bHaveGoods;
};
