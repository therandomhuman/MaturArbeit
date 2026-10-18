// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseMode/AICommanderBase.h"
#include "RTS_Unit_Base.h"
#include "DefenseMode/DefenseGameModeBase.h"
#include "DefenseMode/WaveSpawner.h"
#include "AIControllerBase.h"
#include "Kismet/GameplayStatics.h"


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
		false
	);

	GameMode = Cast<ADefenseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->RegisterAiCommander(this);
	
}

void AAICommanderBase::ProccessNextObjective()
{
	CurrentObjective = GameMode->GetCurrentObjective();
	GetWorldTimerManager().SetTimer(
		AiCommanderTickHandle,
		this,
		&AAICommanderBase::ProccessBattlefield,
		3.f,
		true
	);
	
}

void AAICommanderBase::AddUnitGroups(TArray<FAiUnitGroup> NewUnitGroups)
{
	CurrentlyActiveUnitGroups.Append(NewUnitGroups);
	for (auto UnitGroup : NewUnitGroups)
	{
		for (auto Unit : UnitGroup.Units)
		{
			AController* UnitControllerGeneric = Unit->GetController();
			AAIControllerBase* AiUnitController = Cast<AAIControllerBase>(UnitControllerGeneric);
			AiUnitController->SensedEnemyDelegate.AddUObject(this, &AAICommanderBase::RegisterNewEnemy);
			Unit->UnitDeathDelegate.AddUObject(this, &AAICommanderBase::UnitCleanUp);
		}
	}
}

void AAICommanderBase::SetWaveSpawner(AWaveSpawner* Spawner)
{
	WaveSpawner = Spawner;
	WaveSpawner->WaveDelegate.AddUObject(this, &AAICommanderBase::AddUnitGroups);
	WaveSpawner->RegisterAiCommander(this);
}

int32 AAICommanderBase::ReturnNumberOfActiveUnitGroups()
{
	return CurrentlyActiveUnitGroups.Num();
}


void AAICommanderBase::IssueMoveOrder(FAiUnitGroup& UnitGroupToCommand,FVector Goal)
{
	for (auto& Unit : UnitGroupToCommand.Units)
	{
		Unit->Execute_MoveToLocation(Unit,Goal);
	}
}

void AAICommanderBase::IssueAttackOrder(FAiUnitGroup& UnitGroupToCommand, AActor* Goal)
{
	for (auto& Unit : UnitGroupToCommand.Units)
	{
		Unit->Execute_AttackObject(Unit,Goal);
	}
}

void AAICommanderBase::ProccessBattlefield()
{

	for (auto& UnitGroup : CurrentlyActiveUnitGroups)
	{
		if (UnitGroup.CurrentActivity == FGameplayTag::EmptyTag)
		{
			ProccessUnit(UnitGroup);
		}
	}
}

void AAICommanderBase::ProccessUnit(FAiUnitGroup& UnitGroupToCommand)
{
	if (TrackedEnemies.Num() > 0)
	{
		AActor* ShortestDistanceTo = CurrentObjective.RelevantObject;
		int32 Index = 0;
		int32 TrackedIndex = -1;//negative number in case no index is tracked
		for (auto& TrackedEnemy : TrackedEnemies)
		{
			double DistanceToEnemy = (TrackedEnemy.TrackedUnit->GetActorLocation() - UnitGroupToCommand.Units[0]->GetActorLocation()).Size();
			if (DistanceToEnemy<(ShortestDistanceTo->GetActorLocation() - UnitGroupToCommand.Units[0]->GetActorLocation()).Size())
			{
				ShortestDistanceTo = TrackedEnemy.TrackedUnit;
				TrackedIndex = Index;
			}
			Index++;
		}
		if (TrackedIndex >= 0)
		{
			TrackedEnemies[TrackedIndex].EngagedFriendlies.Append(UnitGroupToCommand.Units);
			//issue attack order here
			IssueAttackOrder(UnitGroupToCommand, TrackedEnemies[TrackedIndex].TrackedUnit);
			return;
		}
	}
		IssueMoveOrder(UnitGroupToCommand, CurrentObjective.RelevantObject->GetActorLocation());
		UnitGroupToCommand.CurrentActivity = FGameplayTag::RequestGameplayTag("UnitStatus.OccupyingObjective");
}

void AAICommanderBase::RegisterNewEnemy(TObjectPtr<AActor> NewEnemy, bool bSensed)
{
	if (bSensed)
	{
		FTrackedEnemy NewTrackedEnemy;
		NewTrackedEnemy.TrackedUnit = NewEnemy;
		TrackedEnemies.Add(NewTrackedEnemy);
	}
	else
	{
		TrackedEnemies.RemoveAll(
			[NewEnemy](const FTrackedEnemy& Enemy)
			{
				return Enemy.TrackedUnit == NewEnemy;
			}
		);
	}
}

void AAICommanderBase::UnitCleanUp(AActor* DeadUnit)
{

	ARTS_Unit_Base* DeadUnitClass = Cast<ARTS_Unit_Base>(DeadUnit);
	if (DeadUnitClass)
	{
		int32 i = 0;
		for (auto& UnitGroup : CurrentlyActiveUnitGroups)
		{
			UnitGroup.Units.Remove(DeadUnitClass);
			if (UnitGroup.Units.Num() <= 0)
			{
				CurrentlyActiveUnitGroups.RemoveAt(i);
				break;
			}
			i++;
		}
	}
}

// Called every frame
void AAICommanderBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

