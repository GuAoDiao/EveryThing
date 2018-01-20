#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "EveryThingTypes.generated.h"

UENUM(BlueprintType)
enum class EElementType : uint8
{
	None,
	Fire,
	Water
};

UENUM(BlueprintType)
enum class EPromptType : uint8
{
	None,
	Error,
	Message,
	Tips
};

USTRUCT(BlueprintType)
struct FInputAxis : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName AxisName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Input;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AxisVlaue;
};

USTRUCT(BlueprintType)
struct FInputAction : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ActionName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Input;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bInShift;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bInCtrl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bInAlt;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bInCmd;
};