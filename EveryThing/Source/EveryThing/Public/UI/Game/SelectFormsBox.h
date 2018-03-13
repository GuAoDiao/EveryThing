// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EveryThingTypes.h"
#include "SelectFormsBox.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API USelectFormsBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent)
	void InitializeSelectFormsBoxDisplay(const FName& TargetRoleName);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSelectFormsBoxDisplay(const TArray<FName>& AllHaveRoleNames);

	UFUNCTION(BlueprintImplementableEvent)
	void AddSelectItem(class USelectItem* SelectItem);


	void OnUpdatePlayerInfo(const FPlayerInfo& InPlayerInfo);	
};
