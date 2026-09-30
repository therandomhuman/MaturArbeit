// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseMode/AICommanderBase.h"
#include "RTS_Unit_Base.h"
#include "DefenseMode/DefenseGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/WaveSystem.h"

// Sets default values
AAICommanderBase::AAICommanderBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


}

// Called when the game starts or when spawned
void AAICommanderBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		AiCommanderTickHandle,
		this,
		&AAICommanderBase::ProccessNextObjective,
		5.f,
		true
	);

}

void AAICommanderBase::ProccessNextObjective()
{
	FObjective CurrentObject = Cast<ADefenseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetCurrentObjective();
	
	IssueMoveOrder(CurrentlyActiveUnitGroups[0], CurrentObject.RelevantObject->GetActorLocation());
}

void AAICommanderBase::IssueMoveOrder(FAiUnitGroup UnitGroupToCommand,FVector Goal)
{
	for (auto& Unit : UnitGroupToCommand.Units)
	{
		Unit->Execute_MoveToLocation(Unit,Goal);
	}
}

// Called every frame
void AAICommanderBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAICommanderBase::SpawnWave()
{

}

