// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"


class UAIPerceptionComponent;

#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class RTS_UTILS_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
	



private:

	AAIControllerBase();

	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly,meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AiPerceptionComp = nullptr;
};
