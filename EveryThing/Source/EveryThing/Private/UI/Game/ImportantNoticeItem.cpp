// Fill out your copyright notice in the Description page of Project Settings.

#include "ImportantNoticeItem.h"


void UImportantNoticeItem::InitializeImportantNoticeItem_Implementation(const FText& ImportantNoteText, EImportantNoteType NoteType, float InRamainingTime)
{
	RemainingTime = InRamainingTime;
}