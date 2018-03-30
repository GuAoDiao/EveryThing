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
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapPath;
};

USTRUCT(BlueprintType)
struct FMapTypeInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Type;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 AllowedTeamNum;
};

//////////////////////////////////////////////////////////////////////////
/// Game Pawn
USTRUCT(BlueprintType)
struct FGamePawnInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	// _�;ö�(HP)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxDurability;
	// _����(_����)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxStamina;
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
	float MaxEnergy;

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
	TArray<FName> AllHaveRoleNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> AllHaveRoleSkinNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> AllHaveRoleFormNames;
};


//////////////////////////////////////////////////////////////////////////
/// Role Info
USTRUCT(BlueprintType)
struct FRoleInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftClassPtr<class AGamePawn> RoleClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Cost;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName DefaultSkinName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName DefaultFormName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct FRoleSkinInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RoleName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Cost;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct FRoleFormInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RoleName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Cost;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct FRoleDisplayInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName RoleName;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName DefaultSkinName;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FRotator DisplayRotation;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector DisplayScale;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class UStaticMesh* DisplayStaticMesh;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkeletalMesh* DisplaySkeletalMesh;
};