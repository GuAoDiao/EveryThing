// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "ChatWIndow/ChatWindowGameStateInterface.h"

#include "EveryThingGameState.generated.h"


class AEveryThingPlayerState;

/**
 * 
 */
UCLASS()
class EVERYTHING_API AEveryThingGameState : public AGameStateBase, public IChatWindowGameStateInterface
{
	GENERATED_BODY()
	
public:
	virtual void HandleBeginPlay() override;
	
	//////////////////////////////////////////////////////////////////////////
	/// Player Add And Leave
public:
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	//////////////////////////////////////////////////////////////////////////
	/// For Chat Window Game State Interface
public:
	virtual const FString& GetPlayerChatName(int32 PlayerID) const override;
	virtual const TArray<APlayerState*>& GetAllChatPlayerState() const override { return AllChatPlayerState; }

	const static FString PlayerChatName_NONE;

	UPROPERTY(Transient, Replicated)
	TArray<APlayerState*> AllChatPlayerState;
};
