// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagAssetInterface.h"

class UAIPerceptionComponent;
struct FAIStimulus;
class APawn;
class ARTS_Unit_Base;



DECLARE_MULTICAST_DELEGATE_TwoParams(FSensedEnemyDelegate, TObjectPtr<AActor> /* sensed enemy */, bool bSensed/*if the enemy was sensed or lost*/);


#include "AIControllerBase.generated.h"
struct FGameplayTagContainer;
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

	virtual void OnPossess(APawn* InPawn) override;


	UPROPERTY(EditDefaultsOnly,meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AiPerceptionComp = nullptr;

	UFUNCTION()
	void PerceptionTargetUpdate(AActor* Actor, FAIStimulus Stimulus);

	TObjectPtr<APawn> PossesedPawn = nullptr;

	TObjectPtr<ARTS_Unit_Base> PossedUnit = nullptr;




	UPROPERTY()
	FGameplayTagContainer OwnerTags;


public:
	FSensedEnemyDelegate SensedEnemyDelegate;

};
