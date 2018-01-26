// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HouseCreate.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UHouseCreate : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHouseCreate(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void HostGame(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum);

	UFUNCTION(BlueprintCallable)
	void BackUp();
	
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllMaps(const FString& MapType) const;

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> MapsType;
private:
	class UDataTable* MapsInfoDataTable;
};
