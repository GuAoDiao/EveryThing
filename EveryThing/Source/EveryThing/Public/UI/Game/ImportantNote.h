// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EveryThingTypes.h"

#include "ImportantNote.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UImportantNote : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void InitializeImportantNote(const FText& ImportantNoteText, EImportantNoteType NoteType, float InRamainingTime);
	
	float RemainingTime;
};
