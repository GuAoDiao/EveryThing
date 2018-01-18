// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Moves/MovesComponent.h"
#include "SkillComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVERYTHING_API USkillComponent : public UMovesComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

	inline FMoves* GetFirstSkillSkilledness() { return &FirstSkill; }
	inline FMoves* GetSecondSkillSkilledness() { return &SecondSkill; }
	inline FMoves* GetUltimateSkillSkilledness() { return &UltimateSkill; }

protected:
	virtual void StartFirstSkill();
	virtual void StopFirstSkill();
	virtual void ExcuteFirstSkill(float AxisValue);
	void RebindFirstSkill();

	virtual void StartSecondSkill();
	virtual void StopSecondSkill();
	virtual void ExcuteSecondSkill(float AxisValue);
	void RebindSecondSkill();

	virtual void StartUltimateSkill();
	virtual void StopUltimateSkill();
	virtual void ExcuteUltimateSkill(float AxisValue);
	void RebindUltimateSkill();

	virtual void RebindAll() override;
protected:
	FMoves FirstSkill;
	FMoves SecondSkill;
	FMoves UltimateSkill;
};
