// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "DataAssets/GunDataBase.h"
//includde generated engine stuff
#include "CommonUnitDataTypes.generated.h"

//This script includes all the common data types the unit uses, they're seperated from the unit script for easier readability and so that if a script only needs to know the data type it doesn't have to include the entire unit script.
class UInstancedStaticMeshComponent;

UENUM()
enum class EUnitType
{
	Infantry,
	Mountaineer,

};
UENUM()
enum class ESoldierType
{
	Enlisted,
	Officer,

};
USTRUCT()//struct to store pathfinding modifactions
struct FPathfindingModification
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector AvoidanceModification = FVector::ZeroVector;

};



//struct for storing per unit gun data, needs this because the actual data assets are singlton like
USTRUCT()
struct FGunData {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWeaponData> GunDataAsset = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 CurrentAmmo = 0;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 Index = 0;


	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bCanFire = true;

	//to store the ism that stores the visuals, easier than having to search it every time I want to edit
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> WeaponISM = nullptr;

};


USTRUCT()//struct to store soldier information
struct FSoldierPropertyStruct
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bBusy = false;

	UPROPERTY(EditDefaultsOnly, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	int32 UnitHealth = 100;

	//array index in the ISM
	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	int32 Index = 0;

	//Placeholder Variable for storing soldier names. 
	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	FString UnitName = "Max Mustermann";


	
	UPROPERTY(EditDefaultsOnly, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	ESoldierType SoldierType = ESoldierType::Enlisted;

	//Variavble to hold relative position in the unit, this isn't an absolute but more of a around here
	UPROPERTY(EditDefaultsOnly, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	FVector FormationOffSet;

	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	FPathfindingModification PathMods;

	UPROPERTY(EditAnywhere, Category = "Soldier|Weapon", meta = (AllowPrivateAccess = "true"))
	FGunData SoldierWeapon;


};



/**
 * 
 */

class RTS_UTILS_API CommonUnitDataTypes
{

public:


};
