// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

#include "CapturePointBase.generated.h"



class USphereComponent;
struct 	FGameplayTag;

UCLASS()
class RTS_UTILS_API ACapturePointBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACapturePointBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shape, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereComponent = nullptr;

	//used for storing for how many units of the owing team are here
	UPROPERTY(VisibleAnywhere, Category = Units, meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<AActor>> FriendlyUnits;

	//used for storing for how many units of the attacking team are here
	UPROPERTY(VisibleAnywhere, Category = Units, meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<AActor>> EnemyUnits;


	//the owing team, it's a tag so I can easily compare
	UPROPERTY(EditAnywhere, Category = CaptureSystem, meta = (AllowPrivateAccess = "true"))
	FGameplayTag OwningTeam;

	UPROPERTY(EditAnywhere, Category = CaptureSystem, meta = (AllowPrivateAccess = "true"))
	FGameplayTag CurrentlyAtCapAdvantage;

	//the amount of "ticks" the units have to do before a point is caputred/uncaptured
	UPROPERTY(EditAnywhere, Category = CaptureSystem, meta = (AllowPrivateAccess = "true"))
	float MaxCapLevel = 100;

	//if this is == maxcaplevel the point is fully captured, if it's 0 the point is neutral 
	UPROPERTY(EditAnywhere, Category = CaptureSystem, meta = (AllowPrivateAccess = "true"))
	float CurrentCapLevel = MaxCapLevel;


	UPROPERTY(EditAnywhere, Category = CaptureSystem)
	float CaptureInterval = 1.f;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	FTimerHandle CaptureTimerHandle;

	void UpdateCapture();
	void RecalcPointOwnership(bool bEnemyCap);
	void ToggleTimer(bool state);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
