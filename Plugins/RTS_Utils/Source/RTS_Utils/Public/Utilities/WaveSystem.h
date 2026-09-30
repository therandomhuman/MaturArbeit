// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utilities/CommonUnitDataTypes.h"
/**
 * 
 */
class RTS_UTILS_API WaveSystem
{
public:
	WaveSystem();
	~WaveSystem();

	const TArray<FAiUnitGroup> SpawnWave();
};
