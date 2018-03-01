// Fill out your copyright notice in the Description page of Project Settings.

#include "ImportantNote.h"


void UImportantNote::InitializeImportantNote_Implementation(const FText& ImportantNoteText, EImportantNoteType NoteType, float InRamainingTime)
{
	RemainingTime = InRamainingTime;
}