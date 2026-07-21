// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTS_Unit_Base.h"
#include "Components//CapsuleComponent.h"
#include "Components//InstancedStaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

//Including my Utility Function Libaries 
#include "Utilities/MovementUtilities.h"

#include "NavigationSystem.h"

// Sets default values
ARTS_Unit_Base::ARTS_Unit_Base()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapulseComponent"));
	RootComponent = CapsuleComp;
	CapsuleComp->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	//Create IsmComp
	IsmComp = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Main ISM"));
	IsmComp->SetupAttachment(RootComponent);
	//setup selection indicator
	SelectionBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionIndicator"));
	SelectionBox->SetupAttachment(RootComponent);
	SelectionBox->SetHiddenInGame(true);
	SelectionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Create FloatingPawnMovComp
	FloatingPawnMov = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovComp"));


	//Set defaults for the formation offsets, yes this isn't clean. I sort of don't care
	//I use emplace instead of add because add first creates the value and then moves or copies it into the array, emplace places it directly, it's functionally the same but emplace is cleaner
	DefaultFormationOffsets.Emplace(FVector(200, 0, 0));
	DefaultFormationOffsets.Emplace(FVector(0, 0, 0));
	DefaultFormationOffsets.Emplace(FVector(0, 200, 0));
	DefaultFormationOffsets.Emplace(FVector(-200, 0, 0));
	DefaultFormationOffsets.Emplace(FVector(0, -200, 0));


	//assign the formation offsets to soldiers
	for (int i = 0; i < 5; ++i)
	{

		SoldierProperties.Emplace(FSoldierPropertyStruct());

		SoldierProperties[i].FormationOffSet = DefaultFormationOffsets[i];
	}

}

// Called when the game starts or when spawned
void ARTS_Unit_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARTS_Unit_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
}


void ARTS_Unit_Base::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);


	
	NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	TArray<FTransform> Transforms = MovementUtilities::DistributeUnitsAtArea(*NavSys, GetActorLocation(), SoldierProperties);
	Transforms.Reserve(5);
	/*
	for (int i = 0; i < 5; ++i)
	{
		if (!NavSys)
		{
			return;
		}
	

		FNavLocation NavPos;
		FRotator Rot = FRotator::ZeroRotator;
		FVector Scale(1.0f);
		FVector Pos;
		if (NavSys->GetRandomPointInNavigableRadius(GetActorLocation() + SoldierProperties[i].FormationOffSet, 50.f, NavPos))
		{


			Pos = NavPos.Location - GetActorLocation();

		}
		else
		{
			Pos = FVector(0, 0,0);
		}
		UE_LOG(LogTemp, Warning, TEXT("Vector Location: %s"), *Pos.ToString());
		Transforms.Emplace(Rot, Pos, Scale);
	}
	*/
	
	IsmComp->ClearInstances();
	IsmComp->AddInstances(Transforms, true);
	
}

// Called to bind functionality to input
void ARTS_Unit_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ARTS_Unit_Base::SelectActor_Implementation(const bool Select)
{
	SelectionBox->SetHiddenInGame(!Select);
}

void ARTS_Unit_Base::MoveToLocation_Implementation(const FVector TargetLocation)
{
	MoveTargetLocation = TargetLocation + FVector(0, 0, GetDefaultHalfHeight());
	bMoving = true;

	AAIController* PawnAIController = Cast<AAIController>(GetController());
	PawnAIController->MoveToLocation(TargetLocation, 50.f,true, true);
}

void ARTS_Unit_Base::Move()
{

	if (!bMoving)
	{
		return;

	}

	FVector MoveDirection = (MoveTargetLocation - GetActorLocation());
	if (MoveDirection.Length() < MoveTolerance)
	{
		bMoving = false;
		return;
	}

	MoveDirection.Normalize(1);
	//AddMovementInput(MoveDirection, 1.f);

	FRotator DesiredRotation = UKismetMathLibrary::MakeRotFromX(MoveDirection);
	DesiredRotation.Pitch = 0;
	DesiredRotation.Roll = 0;
	
	FRotator NewRoation = FMath::RInterpTo(GetActorRotation(), DesiredRotation, GetWorld()->GetDeltaSeconds(),5.f);
	SetActorRotation(NewRoation);
}
