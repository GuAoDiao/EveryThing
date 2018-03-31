// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreBox.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UScoreBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void OnPlayerStateAdd(class APlayerState* NewPS);
	void OnPlayerStateRemove(class APlayerState* RemovePS);

	// _∞Ÿ∂»∑≠“Î_ .It shows the score information of players by blocks, usually according to team assignment.
	// Min is 1, Max is actual team Num.
	UFUNCTION(BlueprintImplementableEvent)
	void SetSectionNums(int32 BlockNums);

	UFUNCTION(BlueprintImplementableEvent)
	void AddScoreItem(class UScoreItem* ScoreItem, int32 ScoreSection);
	UFUNCTION(BlueprintImplementableEvent)
	void RemoveScoreItem(class UScoreItem* ScoreItem, int32 ScoreSection);
protected:
	TMap<APlayerState*, UScoreItem*> AllScoreItem;
};
