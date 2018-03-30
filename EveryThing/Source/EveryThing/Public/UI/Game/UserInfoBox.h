// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EveryThingTypes.h"

#include "UserInfoBox.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UUserInfoBox : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeCurrentRoleName(const FName& RoleName);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerInfo(const FPlayerInfo& PlayerInfo);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDurability(float Durability, float MaxDurability);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStamina(float Stamina, float MaxStamina);

	void OnRoleNameUpdate(const FName& RoleName);
	void OnUpdatePlayerInfo(const FPlayerInfo& PlayerInfo) { UpdatePlayerInfo(PlayerInfo); }
	void OnPlayerStateUpdate(class APlayerState* PlayerState);

	void OnDurabilityUpdate(float InDurability) { Durability = InDurability; UpdateDurability(Durability, MaxDurability); }
	void OnStaminaUpdate(float InStamina) { Stamina = InStamina; UpdateStamina(Stamina, MaxStamina); }

	void OnMaxDurabilityUpdate(float InMaxDurability) {MaxDurability = InMaxDurability; UpdateDurability(Durability, MaxDurability);}
	void OnMaxStaminaUpdate(float InMaxStamina) {MaxStamina = InMaxStamina; UpdateStamina(Stamina, MaxStamina);}
	
protected:
	float Durability;
	float MaxDurability;
	float Stamina;
	float MaxStamina;
};
