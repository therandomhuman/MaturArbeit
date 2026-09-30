// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utilities/CommonUnitDataTypes.h"
#include "WaveSpawner.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FWaveDelegate, TArray<FAiUnitGroup> /* spawned in units */);

USTRUCT()
struct FSpawnInformation
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = Wave, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARTS_Unit_Base> UnitTypeToSpawn;
	UPROPERTY(EditAnywhere, Category = Wave, meta = (AllowPrivateAccess = "true"))
	int32 AmountToSpawn = 0;
	
};
USTRUCT()
struct FWaveInformation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Wave, meta = (AllowPrivateAccess = "true"))
	TArray<FSpawnInformation> SpawnSpecification;

	UPROPERTY(EditAnywhere, Category = Wave, meta = (AllowPrivateAccess = "true"))
	int32 TimeUntilNextWave = 0;
};


UCLASS()
class RTS_UTILS_API AWaveSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category = Wave, meta = (AllowPrivateAccess = "true"))
	TArray<FWaveInformation> WaveInformation;
	

	UPROPERTY(EditAnywhere, Category = Wave, meta = (AllowPrivateAccess = "true"))
	int32 CurrentWave = 1;


	UPROPERTY(EditAnywhere, Category = Wave, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> SpawnLocations;

	void SpawnWave();


	FTimerHandle WaveTimerHandle;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FWaveDelegate WaveDelegate;
	
	void StartUpWaves();

};
