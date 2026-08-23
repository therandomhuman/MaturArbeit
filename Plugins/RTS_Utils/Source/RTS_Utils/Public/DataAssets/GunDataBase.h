// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "WeaponSystem/BaseProjectile.h"

#include "GunDataBase.generated.h"


/**
 * 
 */
UCLASS()
class RTS_UTILS_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "General Info")
	FString WeaponName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UStaticMesh> WeaponAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Combat")
	int32 Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Combat")
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	FVector DefaultOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Combat")
	float FireRate;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float ReloadTime;




	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Projectile")
	TSubclassOf<ABaseProjectile> ProjectileVisual;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Projectile")
	float Gravity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Projectile")
	float MuzzleVelocity;

};
