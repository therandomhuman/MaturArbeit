// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseMode/WaveSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "DefenseMode/WaveSpawnPoint.h"
#include "RTS_Unit_Base.h"

// Sets default values
AWaveSpawner::AWaveSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


}

// Called when the game starts or when spawned
void AWaveSpawner::BeginPlay()
{
	Super::BeginPlay();

	//collect all wavespawnlocations
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AWaveSpawnPoint::StaticClass(),
		SpawnLocations
	);

}

void AWaveSpawner::SpawnWave()
{
	//since 0 indexed, but Curwave starts at 1
	FWaveInformation& ThisWave = WaveInformation[CurrentWave-1];
	CurrentWave += 1;
	//spawn units and sort them into the groups
	TArray<FAiUnitGroup> NewUnitGroups;
	for (auto SpawnInformation : ThisWave.SpawnSpecification)
	{
		FAiUnitGroup UnitGroup;
		for (int i = 0; i < SpawnInformation.AmountToSpawn; i++) {
			int32 randomindex = FMath::RandRange(0,SpawnLocations.Num() - 1);
			ARTS_Unit_Base* NewAi = GetWorld()->SpawnActor<ARTS_Unit_Base>(SpawnInformation.UnitTypeToSpawn, SpawnLocations[randomindex]->GetActorLocation(),FRotator::ZeroRotator);
			UnitGroup.Units.Add(NewAi);
		}
		NewUnitGroups.Add(UnitGroup);
	}
	WaveDelegate.Broadcast(NewUnitGroups);
	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&AWaveSpawner::SpawnWave,
		ThisWave.TimeUntilNextWave
	);
}

// Called every frame
void AWaveSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaveSpawner::StartUpWaves()
{
	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&AWaveSpawner::SpawnWave,
		1.f
	);
}



