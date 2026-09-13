// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTS_Unit_Base.h"
#include "Components//CapsuleComponent.h"
#include "Components//InstancedStaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

#include "NavMesh/NavMeshPath.h"



//Including my Utility Function Libaries 
#include "Utilities/MovementUtilities.h"
//Include the common data types
#include "Utilities/CommonUnitDataTypes.h"

//include my custom gunhandler comp
#include "GunHandler.h"

#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavFilters/NavigationQueryFilter.h"

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

	//create gun ism
	ISMWeaponSoldier = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Soldier ISM"));
	ISMWeaponSoldier->SetupAttachment(RootComponent);
	ISMWeaponOfficer = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Officer ISM"));
	ISMWeaponOfficer->SetupAttachment(RootComponent);
	//setup selection indicator
	SelectionBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionIndicator"));
	SelectionBox->SetupAttachment(RootComponent);
	SelectionBox->SetHiddenInGame(true);
	SelectionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Create FloatingPawnMovComp
	FloatingPawnMov = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovComp"));


	GunHandler = CreateDefaultSubobject<UGunHandler>(TEXT("GunHandler"));

	GunHandler->SetUpComponent();
	//Set defaults for the formation offsets, yes this isn't clean. I sort of don't care
	//I use emplace instead of add because add first creates the value and then moves or copies it into the array, emplace places it directly, it's functionally the same but emplace is cleaner
	DefaultFormationOffsets.Emplace(FVector(200, 0, -CapsuleComp->GetScaledCapsuleHalfHeight()));
	DefaultFormationOffsets.Emplace(FVector(0, 0, -CapsuleComp->GetScaledCapsuleHalfHeight()));
	DefaultFormationOffsets.Emplace(FVector(0, 200, -CapsuleComp->GetScaledCapsuleHalfHeight()));
	DefaultFormationOffsets.Emplace(FVector(-200, 0, -CapsuleComp->GetScaledCapsuleHalfHeight()));
	DefaultFormationOffsets.Emplace(FVector(0, -200, -CapsuleComp->GetScaledCapsuleHalfHeight()));




	//assign the formation offsets to soldiers

	for (int32 i = 0; i < 5; ++i)
	{

		SoldierProperties.EmplaceAt(i,FSoldierPropertyStruct());

		SoldierProperties[i].FormationOffSet = DefaultFormationOffsets[i];
		SoldierProperties[i].Index = i;
	}



}

// Called when the game starts or when spawned
void ARTS_Unit_Base::BeginPlay()
{
	Super::BeginPlay();
	//Fetch controller and pathfollowing component, this cannot be done in the constructor since they aren't assigned during that time.
	AIController = Cast<AAIController>(GetController());
	PathFollowingComp = AIController->GetPathFollowingComponent();
	PathFollowingComp->OnRequestFinished.AddUObject(this, &ARTS_Unit_Base::HandleMovementCleanUp);
}





void ARTS_Unit_Base::HandleMovementCleanUp(FAIRequestID RequestId, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		bMoving = false;
		UE_LOG(LogTemp, Warning, TEXT("Finished Moving"));
	}

}

void ARTS_Unit_Base::RunAttackCheck()
{
	//just a simple loop that goes through all soldiers, checks if they are able to fire/need to reload and if yes, does so. 
	for (auto &FiringSoldier : SoldierProperties)
	{
		if (FiringSoldier.bBusy)
		{
			continue;
			//just skips a loop if the unit is "busy", that mostly means reloading, but I kept it more ambigious for other usecases
		}
		if (FiringSoldier.SoldierWeapon.CurrentAmmo > 0)
		{
			if (FiringSoldier.SoldierWeapon.bCanFire)
				{

					GunHandler->FireGun(FiringSoldier,TargetActor);
				}
			
		}
		else
		{
			GunHandler->Reload(FiringSoldier);
		}
		
			

	}
}

// Called every frame
void ARTS_Unit_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bMoving)
	{
		Move(DeltaTime);
	}else if (TargetActor != nullptr)
	{
		RunAttackCheck();
	}
	
}


void ARTS_Unit_Base::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);


	
	NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	TArray<FTransform> Transforms = MovementUtilities::DistributeUnitsAtArea(NavSys, GetActorLocation(), SoldierProperties);

	IsmComp->ClearInstances();      
	IsmComp->AddInstances(Transforms, true);
	for (auto &i: SoldierProperties)
	{
		i.SoldierWeapon.CurrentAmmo = i.SoldierWeapon.GunDataAsset->GetDefaultObject<UWeaponData>()->MaxAmmo;
	}
	SoldierProperties = GunHandler->SetUpGuns();
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
	TargetActor = nullptr;
	MoveTargetLocation = TargetLocation;
	bMoving = true;


	
	TObjectPtr<UNavigationPath> NavPath = MovementUtilities::GeneratePathPoints(*NavSys, GetWorld(), GetActorLocation(), MoveTargetLocation, 0.4f, this, NavigationQuery);
	for (auto Point : NavPath->PathPoints)
	{
		
		GetWorld()->SpawnActor<AActor>(DebugPath, FTransform(FRotator::ZeroRotator, Point, FVector::OneVector));
		
	}


	FAIMoveRequest MoveReq(GetActorLocation());
	
	AIController->RequestMove(MoveReq, NavPath->GetPath());


}

void ARTS_Unit_Base::AttackObject_Implementation(AActor* NewTargetActor)
{

	//AIController->StopMovement();
	SetTarget(NewTargetActor);
}


void ARTS_Unit_Base::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(GameplayTags);
}

//health system and damage handling
void ARTS_Unit_Base::TakeDamage(AActor* Causer, int32 Index, int32 Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Dammage Received%i"), Index)
	FSoldierPropertyStruct& HitSoldier = SoldierProperties[Index];

	HitSoldier.UnitHealth -= Damage;
	UnitHealthDelegate.Broadcast(Index, HitSoldier.UnitHealth);
	if (HitSoldier.UnitHealth <= 0)
	{
		RemoveDeadSoldier(HitSoldier);
	}
}


void ARTS_Unit_Base::RemoveDeadSoldier(FSoldierPropertyStruct& DeadSoldier)
{
	UE_LOG(LogTemp, Warning, TEXT("EE%i"), DeadSoldier.Index)
	UE_LOG(LogTemp, Warning, TEXT("EE%i"), SoldierProperties.Num())
	if (DeadSoldier.Index+1 < SoldierProperties.Num())
	{
		for (int i = DeadSoldier.Index+1; i < SoldierProperties.Num() ; ++i)
		{
			SoldierProperties[i].Index -= 1;
			//check if they use the same ISM to store visuals, if yes then also fix their index in the weapon ISM
			if (SoldierProperties[i].SoldierWeapon.WeaponISM == DeadSoldier.SoldierWeapon.WeaponISM)
			{
				SoldierProperties[i].SoldierWeapon.Index -= 1;
			}
		}
	}
	DeadSoldier.SoldierWeapon.WeaponISM->RemoveInstance(DeadSoldier.SoldierWeapon.Index);
	SoldierProperties.RemoveAt(DeadSoldier.Index);
	IsmComp->RemoveInstance(DeadSoldier.Index);
}

//move functionality
void ARTS_Unit_Base::Move(float DeltaTime)
{

	if (!bMoving)
	{
		return;

	}

	
	//AddMovementInput(MoveDirection, 1.f);
	FRotator DesiredRotation = UKismetMathLibrary::MakeRotFromXY(GetVelocity(), GetVelocity());
	DesiredRotation.Pitch = 0;
	DesiredRotation.Roll = 0;

	//FRotator NewRoation = FMath::RInterpTo(GetActorRotation(), DesiredRotation, GetWorld()->GetDeltaSeconds(), 5.f);
	//SetActorRotation(NewRoation);
	TArray<FTransform> NewTransform = MovementUtilities::CalculateMovementTransform(this, DeltaTime);
	IsmComp->BatchUpdateInstancesTransforms(0, NewTransform);
	IsmComp->MarkRenderStateDirty();
	GunHandler->UpdateGuns();
}


const UPathFollowingComponent& ARTS_Unit_Base::GetPathFollowingComp()
{
	return *PathFollowingComp;
}

const TObjectPtr<UInstancedStaticMeshComponent> ARTS_Unit_Base::GetIsm()
{
	return IsmComp;
}

const TArray<FSoldierPropertyStruct> ARTS_Unit_Base::GetUnitdata()
{
	return SoldierProperties;
}

const FSoldierPropertyStruct* ARTS_Unit_Base::GetSoldierData(int32 Index)
{
	if (Index <= SoldierProperties.Num())
	{
		return &SoldierProperties[Index];
	}
	return nullptr;
}



const AActor* ARTS_Unit_Base::GetTarget()
{
	return TargetActor;
}
void ARTS_Unit_Base::SetTarget(AActor* NewTarget)
{
	TargetActor = NewTarget;
	bMoving = false;
	GunHandler->SetUpTarget(NewTarget);
}


void ARTS_Unit_Base::SetSoldierData(int32 Index, FSoldierPropertyStruct NewStruc)
{
	SoldierProperties[Index] = NewStruc;
}

