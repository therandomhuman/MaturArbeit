// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Utilities/CommonUnitDataTypes.h"
#include "GameplayTagContainer.h"
#include "AICommanderBase.generated.h"



class ARTS_Unit_Base;


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

	//since I don't think the commander needs to tick it's general update function every frame I use a timer to make it tick at a slower rate
	FTimerHandle AiCommanderTickHandle;

	//fuctions

	void ProccessNextObjective();


	void IssueMoveOrder(FAiUnitGroup UnitGroupToCommand, FVector Goal);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//gamemode calls commander to spawn wave, it uses the moduel to spawn them in groups and registers them as such 
	//wave information is saved on a data asset on the commander, it uses a unique struct 
	void SpawnWave();
};
