// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseMode/WaveSpawnPoint.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWaveSpawnPoint::AWaveSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	VisualRep = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VIS REP"));
	SetRootComponent(VisualRep);
}

// Called when the game starts or when spawned
void AWaveSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AWaveSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

