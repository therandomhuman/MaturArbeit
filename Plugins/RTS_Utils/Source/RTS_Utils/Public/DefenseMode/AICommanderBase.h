// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Utilities/CommonUnitDataTypes.h"
#include "GameplayTagContainer.h"

#include "DefenseMode/CommonGamemodeDataTypes.h"
#include "AICommanderBase.generated.h"



class ARTS_Unit_Base;
class AWaveSpawner;
class ADefenseGameModeBase;


UCLASS()
class RTS_UTILS_API AAICommanderBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAICommanderBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	//tracks all seen enemies the units have seen
	//maybe also a struct to store more info, like who's already attacking em?????
	UPROPERTY(EditAnywhere, Category = UnitsIntel, meta = (AllowPrivateAccess = "true"))
	TArray<FTrackedEnemy> TrackedEnemies;

	UPROPERTY(EditAnywhere, Category = General, meta = (AllowPrivateAccess = "true"))
	TArray<FAiUnitGroup> CurrentlyActiveUnitGroups;

	UPROPERTY(EditAnywhere, Category = General, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ADefenseGameModeBase> GameMode;


	UPROPERTY(EditAnywhere, Category = General, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWaveSpawner> WaveSpawner = nullptr;
	//since I don't think the commander needs to tick it's general update function every frame I use a timer to make it tick at a slower rate
	FTimerHandle AiCommanderTickHandle;


	FObjective CurrentObjective;

	//fuctions

	void ProccessNextObjective();

	void AddUnitGroups(TArray<FAiUnitGroup>NewUnitGroups);


	void IssueMoveOrder(FAiUnitGroup& UnitGroupToCommand, FVector Goal);

	void IssueAttackOrder(FAiUnitGroup& UnitGroupToCommand, AActor* Goal);

	void ProccessBattlefield();

	void ProccessUnit(FAiUnitGroup& UnitGroupToCommand);


	void RegisterNewEnemy(TObjectPtr<AActor>, bool bSensed);


	//need to clean up before the unit gets removed
	void UnitCleanUp(AActor* DeadUnit);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetWaveSpawner(AWaveSpawner* Spawner);


	int32 ReturnNumberOfActiveUnitGroups();
};
