// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/MovementUtilities.h"
#include "NavigationSystem.h"
#include "RTS_Unit_Base.h"

MovementUtilities::MovementUtilities()
{
	
}

MovementUtilities::~MovementUtilities()
{
}

TArray<FTransform> MovementUtilities::DistributeUnitsAtArea(UNavigationSystemV1& NavSys, FVector const &UnitPosition, TArray<FSoldierPropertyStruct>& SoldierProperties)
{
	TArray<FTransform> Transforms;
	Transforms.Reserve(5);
	for (int i = 0; i < 5; ++i)
	{



		FNavLocation NavPos;
		FRotator Rot = FRotator::ZeroRotator;
		FVector Scale(1.0f);
		FVector Pos;
		if (NavSys.GetRandomPointInNavigableRadius(UnitPosition + SoldierProperties[i].FormationOffSet, 50.f, NavPos))
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
