// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseMode/WaveSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "DefenseMode/WaveSpawnPoint.h"
#include "DefenseMode/DefenseGameModeBase.h"
#include "RTS_Unit_Base.h"
#include "DefenseMode/AICommanderBase.h"

// Sets default values
AWaveSpawner::AWaveSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


}

void AWaveSpawner::RegisterAiCommander(AAICommanderBase* AiCommanderToRegister)
{
	ActiveCommander = AiCommanderToRegister;
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


	AGameModeBase* Gamemode = UGameplayStatics::GetGameMode(GetWorld());
	DefenseGameMode = Cast<ADefenseGameModeBase>(Gamemode);
	DefenseGameMode->RegisterWaveSpawner(this);

}

void AWaveSpawner::SpawnWave()
{
	//since 0 indexed, but Curwave starts at 1
	if (WaveInformation.Num() >= CurrentWave)
	{
		FWaveInformation& ThisWave = WaveInformation[CurrentWave-1];
		CurrentWave += 1;
		//spawn units and sort them into the groups
		TArray<FAiUnitGroup> NewUnitGroups;
		for (auto SpawnInformation : ThisWave.SpawnSpecification)
		{
			FAiUnitGroup UnitGroup;
			for (int i = 0; i < SpawnInformation.AmountToSpawn; i++) {
				int32 randomindex = FMath::RandRange(0,SpawnLocations.Num() - 1);
				ARTS_Unit_Base* NewAi = GetWorld()->SpawnActor<ARTS_Unit_Base>(SpawnInformation.UnitTypeToSpawn, 
					SpawnLocations[randomindex]->GetActorLocation(),FRotator::ZeroRotator);
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
	else
	{
		if (ActiveCommander->ReturnNumberOfActiveUnitGroups() <= 0)
		{
			DefenseGameMode->HandleWaveBeat();

		}else
		{

			GetWorldTimerManager().SetTimer(
				WaveTimerHandle,
				this,
				&AWaveSpawner::SpawnWave,
				2.f
			);
		}
	}
	
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



