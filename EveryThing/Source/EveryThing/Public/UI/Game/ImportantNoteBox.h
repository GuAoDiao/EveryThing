// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EveryThingTypes.h"

#include "ImportantNoteBox.generated.h"

/**
 * 
 */
UCLASS()
class EVERYTHING_API UImportantNoteBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeImportantNoteBox();

	UFUNCTION(BlueprintNativeEvent)
	void AddImportantNoteBox(const FText& ImportantNoteText, EImportantNoteType NoteType, float InRamainingTime = 5.f);
	
	UFUNCTION()
	void OnToggleToTargetRoleSuccess(const FName& TargetRoleName);
	UFUNCTION()
	void OnToggleToTargetRoleFailure(const FName& TargetRoleName, const FText& ErrorInfo);

	UPROPERTY(BlueprintReadWrite)
	class UVerticalBox* ImprotantNoteVerticalBox;

private:
	//////////////////////////////////////////////////////////////////////////
	/// TidyImportantNoteBox
	void TidyImportantNoteBox();

	float TidyRate;
	FTimerHandle TidyImportantNoteBoxTimerHanadle;
};
