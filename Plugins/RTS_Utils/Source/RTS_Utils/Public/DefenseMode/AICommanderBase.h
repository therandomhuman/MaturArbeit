// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Utilities/CommonUnitDataTypes.h"
#include "GameplayTagContainer.h"
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
	UPROPERTY(EditAnywhere, Category = UnitsIntel, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ARTS_Unit_Base>> TrackedEnemies;

	UPROPERTY(EditAnywhere, Category = General, meta = (AllowPrivateAccess = "true"))
	TArray<FAiUnitGroup> CurrentlyActiveUnitGroups;

	UPROPERTY(EditAnywhere, Category = General, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ADefenseGameModeBase> GameMode;


	UPROPERTY(EditAnywhere, Category = General, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWaveSpawner> WaveSpawner = nullptr;
	//since I don't think the commander needs to tick it's general update function every frame I use a timer to make it tick at a slower rate
	FTimerHandle AiCommanderTickHandle;

	//fuctions

	void ProccessNextObjective();

	void AddUnitGroups(TArray<FAiUnitGroup>NewUnitGroups);


	void IssueMoveOrder(FAiUnitGroup UnitGroupToCommand, FVector Goal);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetWaveSpawner(AWaveSpawner* Spawner);
};
