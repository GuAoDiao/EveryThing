// Fill out your copyright notice in the Description page of Project Settings.

#include "ImportantNoticeList.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include "VerticalBox.h"

#include "Online/PlayerController_Game.h"
#include "UI/Game/ImportantNoticeItem.h"
#include "EveryThingAssetManager.h"


#define LOCTEXT_NAMESPACE "EveryThing_GmaeUI_ImportantNoteBox"

void UImportantNoticeList::NativeConstruct()
{
	TidyRate = 0.5f;

	InitializeImportantNoticeList();

	Super::NativeConstruct();
}

void UImportantNoticeList::AddImportantNoticeItem_Implementation(const FText& ImportantNoteText, EImportantNoteType NoteType, float InRamainingTime)
{
	if (!ImprotantNoticeVerticalBox) { return; }

	TSubclassOf<UUserWidget> ImportantNoticeItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("ImportantNoticeItem");
	if (!ImportantNoticeItemClass) { return; }
	
	UImportantNoticeItem* ImportantNoticeItem = CreateWidget<UImportantNoticeItem>(GetOwningPlayer(), ImportantNoticeItemClass);
	if (ImportantNoticeItem)
	{
		ImportantNoticeItem->InitializeImportantNoticeItem(ImportantNoteText, NoteType, InRamainingTime);

		ImprotantNoticeVerticalBox->AddChild(ImportantNoticeItem);

		if (GetWorld())
		{
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			if (!TimerManager.IsTimerActive(TidyTimer))
			{
				TimerManager.SetTimer(TidyTimer, this, &UImportantNoticeList::TidyImportantNoticeList, TidyRate, true);
			}
		}
	}
}




void UImportantNoticeList::TidyImportantNoticeList()
{
	if (ImprotantNoticeVerticalBox)
	{
		for (int32 i = 0, l = ImprotantNoticeVerticalBox->GetChildrenCount(); i < l; ++i)
		{
			UImportantNoticeItem* ImportantNode = Cast<UImportantNoticeItem>(ImprotantNoticeVerticalBox->GetChildAt(i));
			if (ImportantNode)
			{
				UE_LOG(LogTemp, Log, TEXT("-_- Curren ramining Time is : %f"), ImportantNode->RemainingTime);
				ImportantNode->RemainingTime -= TidyRate;
				if (ImportantNode->RemainingTime <= 0.f)
				{
					ImprotantNoticeVerticalBox->RemoveChildAt(i);
					i--;
					l--;
				}
			}
		}

		if (ImprotantNoticeVerticalBox->GetChildrenCount() == 0 && GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(TidyTimer);
		}
	}
}

#undef LOCTEXT_NAMESPACE 