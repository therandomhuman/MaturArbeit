// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveSpawnPoint.generated.h"

class UStaticMeshComponent;
UCLASS()
class RTS_UTILS_API AWaveSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> VisualRep;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
