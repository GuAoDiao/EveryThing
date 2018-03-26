// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "RoleDisplay.generated.h"



UCLASS()
class EVERYTHING_API ARoleDisplay : public AActor
{
	GENERATED_BODY()
	
public:
	ARoleDisplay();
	
	void AdjustUp(float AxisValue);
	void AdjustRight(float AxisValue);


	void ChangeSkin(const FName& SkinName);
	void ChangeRole(const FName& RoleName);

	FName CurrentRoleName;
	FName CurrentSkinName;
	class FGamePawnSkin* CurrentSkin;
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UCameraComponent* CameraComp;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USceneCaptureComponent2D* SceneCaptureComp2D;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComp;
};
