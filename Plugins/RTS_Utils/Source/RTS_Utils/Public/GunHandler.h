// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

class ARTS_Unit_Base;

struct FSoldierPropertyStruct;
#include "GunHandler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_UTILS_API UGunHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunHandler();


	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ARTS_Unit_Base> OwnerPtr = nullptr;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//these two need the ufunction macro so I can bind them to the ftimerdelegate, they others don't really need them
	UFUNCTION()
	void SetCanFire(FSoldierPropertyStruct& FiringSoldier);


	//I need this function for the reload one, but it doesn't hurt having it be more general purpose, you never know?
	UFUNCTION()
	void SetAmmo(FSoldierPropertyStruct& FiringSoldier, int32 Ammo);


	TArray<FSoldierPropertyStruct> SetUpGuns();

	void UpdateGuns();

	void SetUpComponent();


	void FireGun(FSoldierPropertyStruct &FiringSoldier );

	void Reload(FSoldierPropertyStruct& FiringSoldier);


};
