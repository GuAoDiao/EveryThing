// Fill out your copyright notice in the Description page of Project Settings.

#include "ImportantNoteBox.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include "VerticalBox.h"

#include "Characters/PlayerPawnController.h"
#include "UI/Game/ImportantNote.h"
#include "EveryThingAssetManager.h"


#define LOCTEXT_NAMESPACE "EveryThing_GmaeUI_ImportantNoteBox"

void UImportantNoteBox::NativeConstruct()
{
	TidyRate = 0.5f;

	InitializeImportantNoteBox();

	Super::NativeConstruct();
}

void UImportantNoteBox::AddImportantNoteBox_Implementation(const FText& ImportantNoteText, EImportantNoteType NoteType, float InRamainingTime)
{
	if (!ImprotantNoteVerticalBox) { return; }
	TSubclassOf<UUserWidget> ImportantNoteClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("ImportantNote");
	if (!ImportantNoteClass) { return; }
	
	UImportantNote* ImportantNote = CreateWidget<UImportantNote>(GetOwningPlayer(), ImportantNoteClass);
	if (ImportantNote)
	{
		ImportantNote->InitializeImportantNote(ImportantNoteText, NoteType, InRamainingTime);

		ImprotantNoteVerticalBox->AddChild(ImportantNote);

		if (GetWorld())
		{
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			if (!TimerManager.IsTimerActive(TidyImportantNoteBoxTimerHanadle))
			{
				TimerManager.SetTimer(TidyImportantNoteBoxTimerHanadle, this, &UImportantNoteBox::TidyImportantNoteBox, TidyRate, true);
			}
		}
	}
}




void UImportantNoteBox::TidyImportantNoteBox()
{
	if (ImprotantNoteVerticalBox)
	{
		for (int32 i = 0, l = ImprotantNoteVerticalBox->GetChildrenCount(); i < l; ++i)
		{
			UImportantNote* ImportantNode = Cast<UImportantNote>(ImprotantNoteVerticalBox->GetChildAt(i));
			if (ImportantNode)
			{
				UE_LOG(LogTemp, Log, TEXT("-_- Curren ramining Time is : %f"), ImportantNode->RemainingTime);
				ImportantNode->RemainingTime -= TidyRate;
				if (ImportantNode->RemainingTime <= 0.f)
				{
					ImprotantNoteVerticalBox->RemoveChildAt(i);
					i--;
					l--;
				}
			}
		}

		if (ImprotantNoteVerticalBox->GetChildrenCount() == 0 && GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(TidyImportantNoteBoxTimerHanadle);
		}
	}
}

#undef LOCTEXT_NAMESPACE 