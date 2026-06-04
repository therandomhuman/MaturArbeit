// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTS_Unit_Base.h"
#include "Components//CapsuleComponent.h"
#include "Components//SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
// Sets default values
ARTS_Unit_Base::ARTS_Unit_Base()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapulseComponent"));
	RootComponent = CapsuleComp;
	CapsuleComp->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	//Create SkelMeshComp
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMesh->SetupAttachment(RootComponent);
	//setup selection indicator
	SelectionBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionIndicator"));
	SelectionBox->SetupAttachment(RootComponent);
	SelectionBox->SetHiddenInGame(true);
	SelectionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Create FloatingPawnMovComp
	FloatingPawnMov = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovComp"));

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
	PawnAIController->MoveToLocation(TargetLocation, 50.f);
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
