// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UNavigationSystemV1;
class UNavigationQueryFilter;
class UNavigationPath;

class ARTS_Unit_Base;

struct FSoldierPropertyStruct;
/**
 * 
 */
class RTS_UTILS_API MovementUtilities
{
public:
	MovementUtilities();
	~MovementUtilities();
	

	static TArray<FTransform> DistributeUnitsAtArea(UNavigationSystemV1* NavSys, FVector const &UnitPosition, TArray<FSoldierPropertyStruct>& SoldierProperties);


	static TObjectPtr<UNavigationPath> GeneratePathPoints(UNavigationSystemV1& NavSys, UObject* WorldContext,const FVector& PathStart, const FVector& PathEnd, const float OffSetDistance, AActor* PathFindingContext, TSubclassOf <UNavigationQueryFilter> FilterClass);

	static TArray<FTransform> CalculateMovementTransform(TObjectPtr<ARTS_Unit_Base> Unit, float DeltaTime);


};
