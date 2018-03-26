// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EveryThingTypes.h"

#include "Storehouse.generated.h"

class URoleItem;
class USkinItem;
class UFormItem;

/**
 * 
 */
UCLASS()
class EVERYTHING_API UStorehouse : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UStorehouse(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION(BlueprintNativeEvent)
	void InitializeRoleListDisplay(const FPlayerInfo& InPlayerInfo);

	UFUNCTION(BlueprintNativeEvent)
	void InitializeRoleSkinAndFormListDisplay(const FPlayerInfo& InPlayerInfo);


	UFUNCTION(BlueprintImplementableEvent)
	void AddRoleItem(URoleItem* RoleItem);
	UFUNCTION(BlueprintImplementableEvent)
	void AddSkinItem(USkinItem* SkinItem);
	UFUNCTION(BlueprintImplementableEvent)
	void AddFormItem(UFormItem* FormItem);


	UFUNCTION(BlueprintCallable)
	void AdjustDisplayRoleUp(float AxisValue);
	UFUNCTION(BlueprintCallable)
	void AdjustDisplayRoleRight(float AxisValue);

	UFUNCTION(BlueprintCallable)
	void ToggleDisplayRole(const FName& RoleName);
	UFUNCTION(BlueprintCallable)
	void ToggleDisplaySkin(const FName& SkinName);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerInfoUpdate(const FPlayerInfo& InPlayerInfo);

	UFUNCTION(BlueprintCallable)
	void Backup();

protected:
	class ARoleDisplay* RoleDisplay;
	class APlayerController* OwnerPC;
	TMap<FKey, float> AdjustUPKey;
	TMap<FKey, float> AdjustRightKey;
	FName CurrentDisplayRoleName;
};
