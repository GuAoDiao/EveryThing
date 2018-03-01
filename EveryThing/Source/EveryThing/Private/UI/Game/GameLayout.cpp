// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLayout.h"


#include "UI/Game/SelectRolesBox.h"


void UGameLayout::NativeConstruct()
{
	InitializeGameLayout();

	Super::NativeConstruct();
}

void UGameLayout::DisplaySelectRolesBox() { if (SelectRolesBox) { SelectRolesBox->SetVisibility(ESlateVisibility::Visible); } }
void UGameLayout::RemoveSelectRolesBox() { if (SelectRolesBox) { SelectRolesBox->SetVisibility(ESlateVisibility::Hidden); } }