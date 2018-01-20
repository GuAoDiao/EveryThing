// Fill out your copyright notice in the Description page of Project Settings.

#include "ChairPawn.h"

AChairPawn::AChairPawn()
{
	Quality = 1000.f;
	QualityScale = 1.f;

	LinearDamping = 0.01f;
	AngularDamping = 10.f;

	ResetQualityAndDamping();
}