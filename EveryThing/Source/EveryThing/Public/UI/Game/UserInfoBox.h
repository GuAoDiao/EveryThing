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


	void OnRoleNameUpdate(const FName& RoleName) { InitializeCurrentRoleName(RoleName); }
	void OnUpdatePlayerInfo(const FPlayerInfo& PlayerInfo) { UpdatePlayerInfo(PlayerInfo); }
	void OnPlayerStateUpdate(class APlayerState* PlayerState);
};
