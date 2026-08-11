// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RTS_CommandInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URTS_CommandInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTS_UTILS_API IRTS_CommandInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category = "Command interface")
	void MoveToLocation(const FVector TargetLocation);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Command interface")
	void AttackObject( AActor* ActorToAttack);
};
