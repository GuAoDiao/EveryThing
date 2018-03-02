#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "ChatWindowTypes.generated.h"

USTRUCT(BlueprintType)
struct FChatMessageInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Message;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PlayerID;
};