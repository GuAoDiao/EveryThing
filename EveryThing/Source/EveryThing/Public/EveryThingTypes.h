#pragma once

#include "CoreMinimal.h"

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