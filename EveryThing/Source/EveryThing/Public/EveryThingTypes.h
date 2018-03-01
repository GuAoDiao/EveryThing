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
enum class EImportantNoteType : uint8
{
	None,
	Warning,
	Message,
	Error,
	Tips
};

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	None,
	Free,
	FirstTeam,
	SecondTeam,
	ThirdTeam
};

//////////////////////////////////////////////////////////////////////////
/// Input
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


//////////////////////////////////////////////////////////////////////////
/// Maps
USTRUCT(BlueprintType)
struct FMapInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapName;
};


//////////////////////////////////////////////////////////////////////////
/// Gmaw Pawn
USTRUCT(BlueprintType)
struct FGamePawnInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	// _�;ö�(HP)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Durability;
	// _����(_����)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PhysicalPower;
	// _����(_�ٶ�)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Agility;
	// _�ȶ���(_����)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Stability;

	// _��Ұ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHyperopiaDistance;

	
	// _Ԫ������
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<EElementType> ElementType;
	// _Ԫ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EElementType, float> ElementValue;
	// _Ԫ�ؿ���
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EElementType, float> ElementResistance;


	// _����ֵ(_����ֵ)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PowerValue;

	// _����(_��������)
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Quality;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float QualityScale;

	// _����ֵ(_����)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LinearDamping;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AngularDamping;
};


//////////////////////////////////////////////////////////////////////////
/// Player Info
USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PlayerName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Gold;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Experience;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> AllHaveRolesName;
};