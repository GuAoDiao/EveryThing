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
	void HostGame(const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum);

	UFUNCTION(BlueprintCallable)
	void Back();
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> MapsType;

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllMaps(const FString& MapType) const;
private:
	class UDataTable* MapsInfoDataTable;
};
