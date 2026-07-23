// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/MovementUtilities.h"
#include "NavigationSystem.h"
#include "RTS_Unit_Base.h"
#include "NavigationPath.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectBaseUtility.h"
#include "UObject/Object.h"
#include "NavMesh/NavMeshPath.h"

//these are for debuging and will be removed at a later date
#include "Components/StaticMeshComponent.h"


MovementUtilities::MovementUtilities()
{
	
}

MovementUtilities::~MovementUtilities()
{
}

TArray<FTransform> MovementUtilities::DistributeUnitsAtArea(UNavigationSystemV1* NavSys, FVector const &UnitPosition, TArray<FSoldierPropertyStruct>& SoldierProperties)
{
	TArray<FTransform> Transforms;
	Transforms.Reserve(5);
	if (NavSys == nullptr)
	{
		Transforms.Init(FTransform::Identity,5);
		return Transforms;
	}

	for (int i = 0; i < 5; ++i)
	{



		FNavLocation NavPos;
		FRotator Rot = FRotator::ZeroRotator;
		FVector Scale(1.0f);
		FVector Pos;
		if (NavSys->GetRandomPointInNavigableRadius(UnitPosition + SoldierProperties[i].FormationOffSet, 50.f, NavPos))
		{


			Pos = NavPos.Location - UnitPosition;

		}
		else
		{
			Pos = FVector(0, 0, 0);
		}
		UE_LOG(LogTemp, Warning, TEXT("Vector Location: %s"), *Pos.ToString());
		Transforms.Emplace(Rot, Pos, Scale);
	}
	return Transforms;
}

TObjectPtr<UNavigationPath> MovementUtilities::GeneratePathPoints(UNavigationSystemV1& NavSys, UObject* WorldContext, const FVector& PathStart, const FVector& PathEnd, const float OffSetDistance, AActor* PathFindingContext, TSubclassOf <UNavigationQueryFilter> FilterClass)
{
	UNavigationPath* NavPath = NavSys.FindPathToLocationSynchronously(WorldContext,PathStart,PathEnd,PathFindingContext,FilterClass);

	if (!NavPath->IsValid())
	{
		return NavPath;
	}
	const FNavPathSharedPtr SharedPathPtr = NavPath->GetPath();

		SharedPathPtr.Get()->CastPath<FNavMeshPath>()->OffsetFromCorners(150.f);
		UNavigationPath* Offsetpath = NewObject<UNavigationPath>();
		Offsetpath->SetPath(SharedPathPtr);

	



	return Offsetpath;
}

TArray<FTransform> MovementUtilities::CalculateMovementTransform(TObjectPtr<ARTS_Unit_Base> Unit)
{
	//TObjectPtr<UInstancedStaticMeshComponent> ISM = Unit->GetIsm();
	return TArray<FTransform>();
}
