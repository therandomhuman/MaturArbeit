// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UNavigationSystemV1;

struct FSoldierPropertyStruct;
/**
 * 
 */
class RTS_UTILS_API MovementUtilities
{
public:
	MovementUtilities();
	~MovementUtilities();
	

	static TArray<FTransform> DistributeUnitsAtArea(UNavigationSystemV1& NavSys, FVector const &UnitPosition, TArray<FSoldierPropertyStruct>& SoldierProperties);
};
