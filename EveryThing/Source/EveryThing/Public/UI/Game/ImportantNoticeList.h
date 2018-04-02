// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EveryThingTypes.h"

#include "ImportantNoticeList.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UImportantNoticeList : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeImportantNoticeList();

	UFUNCTION(BlueprintNativeEvent)
	void AddImportantNoticeItem(const FText& ImportantNoteText, EImportantNoteType NoteType, float InRamainingTime = 5.f);
	
	UPROPERTY(BlueprintReadWrite)
	class UVerticalBox* ImprotantNoticeVerticalBox;

private:
	//////////////////////////////////////////////////////////////////////////
	/// TidyImportantNoteBox
	void TidyImportantNoticeList();

	float TidyRate;
	FTimerHandle TidyTimer;
};
