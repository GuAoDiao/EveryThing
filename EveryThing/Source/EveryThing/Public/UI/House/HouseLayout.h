// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HouseLayout.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UHouseLayout : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	//////////////////////////////////////////////////////////////////////////
	/// Player Item

	void OnAddPlayer(class APlayerState* PlayerState);
	void OnRemovePlayer(class APlayerState* PlayerState);

	UFUNCTION(BlueprintImplementableEvent)
	void AddHousePlayerItem(class UHousePlayerItem* HousePlayerItem);
	
	UFUNCTION(BlueprintImplementableEvent)
	void RemoveHousePlayerItem(class UHousePlayerItem* HousePlayerItem);

	TMap<APlayerState*, UHousePlayerItem*> AllPlayerItemList;

	//////////////////////////////////////////////////////////////////////////
	/// Player Info
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsHouseOwner(bool bIsHouserOwner);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateReadyState(bool bIsReady);

	UFUNCTION(BlueprintCallable)
	void ToggleReadState();

	UFUNCTION(BlueprintCallable)
	void StartGame();
	
	void OnPlayerStateUpdate(class APlayerState* PlayerState);


	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllMaps(const FString& MapType) const;

	UFUNCTION(BlueprintCallable)
	const TArray<FString>& GetAllMapsType() const;

	UFUNCTION(BlueprintCallable)
	void UpdateHouseSetting(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, int32 MaxPlayersNum);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateHouseSetting(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, int32 MaxPlayersNum);
};