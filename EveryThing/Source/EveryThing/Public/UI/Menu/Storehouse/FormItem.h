// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Storehouse/GoodItem.h"
#include "FormItem.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UFormItem : public UGoodsItem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void InitializeFormItem(const FName& InFormName, int32 InCost, bool bInHaveGoods);
	
	UFUNCTION(BlueprintNativeEvent)
	void InitializeFormItemDisplay(const FName& InFormName, bool bInHaveGoods);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateFormItemDisplay(bool bInHaveGoods);

	UFUNCTION(BlueprintPure)
	const FName& GetFormName() { return FormName; }

	UFUNCTION(BlueprintCallable)
	void OnBuyFormItem();
	
	virtual bool BuyGoodsItem() override;
private:
	FName FormName;	
};
