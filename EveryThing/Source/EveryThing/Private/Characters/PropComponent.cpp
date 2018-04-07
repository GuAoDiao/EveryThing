// Fill out your copyright notice in the Description page of Project Settings.

#include "PropComponent.h"

#include "UnrealNetwork.h"

#include "EveryThingAssetManager.h"
#include "Characters/GamePawn.h"
#include "SceneObject/Prop/PropBase.h"

UPropComponent::UPropComponent()
{
	bReplicates = true;

	MaxPropItemNum = 4;
	CurrentPropItemNum = 0;

	OwnerGamePawn = Cast<AGamePawn>(GetOwner());

	for (int32 i = 0; i < MaxPropItemNum; ++i)
	{
		AllPropsIndex.Add(-1);
	}
}



//////////////////////////////////////////////////////////////////////////
/// Game Prop
bool UPropComponent::AddProp(int32 PropID)
{
	if (AllProps.Contains(PropID))
	{
		++AllProps[PropID];
		OnAllPropsChanged();
		return true;
	}
	else
	{
		if (CurrentPropItemNum <= MaxPropItemNum)
		{
			AllProps.Add(PropID, 1);
			++CurrentPropItemNum;
			for (int32 i = 0; i < MaxPropItemNum; ++i)
			{
				if (AllPropsIndex[i] == -1)
				{
					AllPropsIndex[i] = PropID;
					break;
				}
			}
			OnAllPropsIndexChanged();
			return true;
		}
	}

	return false;
}

void UPropComponent::UsePropFromIndex(int32 Index)
{
	if (AllPropsIndex.IsValidIndex(Index))
	{
		UseProp(AllPropsIndex[Index]);
	}
}

void UPropComponent::UseProp(int32 PropID)
{
	if (AllProps.Contains(PropID))
	{
		const FPickupPropInfo* PropInfo = UEveryThingAssetManager::GetAssetManagerInstance()->GetPropInfoFromID(PropID);
		
		UPropBase* Prop = PropInfo && PropInfo->PropClass ? NewObject<UPropBase>(this, PropInfo->PropClass) : nullptr;
		if (Prop && Prop->BeUseByGamePawn(OwnerGamePawn))
		{
			--AllProps[PropID];

			OnAllPropsChanged();

			if (AllProps[PropID] == 0)
			{
				--CurrentPropItemNum;
				AllProps.Remove(PropID);
				for (int32 i = 0; i < MaxPropItemNum; ++i)
				{
					if (AllPropsIndex[i] == PropID)
					{
						AllPropsIndex[i] = -1;
						break;
					}
				}
				OnAllPropsIndexChanged();
			}
		}		
	}
}

int32 UPropComponent::GetPropNums(int32 PropID) const
{
	return AllProps.Contains(PropID) ? AllProps[PropID] : 0;
}


void UPropComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPropComponent, AllPropsIndex);
	DOREPLIFETIME(UPropComponent, AllProps);
}