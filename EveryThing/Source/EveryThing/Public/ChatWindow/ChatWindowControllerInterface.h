// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "ChatWindowControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UChatWindowControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EVERYTHING_API IChatWindowControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual class UChatComponent* GetChatComponent() const = 0;
};
