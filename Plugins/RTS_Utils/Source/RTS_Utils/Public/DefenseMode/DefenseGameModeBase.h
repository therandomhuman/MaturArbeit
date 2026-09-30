// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CaptureSystem/CapturePointBase.h"
#include "DefenseGameModeBase.generated.h"

class ACapturePointBase;
class AHUDBase;
struct FGameplayTag;
/**
 * 
 */
UENUM()
enum EObjectiveType
{
	Capture
};

USTRUCT()
struct FObjective
{
	GENERATED_BODY()
	EObjectiveType ObjectiveType = EObjectiveType::Capture;

	AActor* RelevantObject = nullptr;
};
UCLASS()
class RTS_UTILS_API ADefenseGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere,Category = "Points", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACapturePointBase> CapturePointAssigned = nullptr;

	UPROPERTY(EditAnywhere, Category = "Team", meta = (AllowPrivateAccess = "true"))
	FGameplayTag PlayerTeam;

	TArray<AActor*> CapturePoints;
	UFUNCTION()
	void HandlePointCapture(const FGameplayTag& Capturer);


	UPROPERTY()
	TObjectPtr<AHUDBase> ActiveHUD = nullptr;

	FObjective CurrentObjective;
	 
public:

	//getter function so that e.g the ai commander knows the current objective, I return a struct so I can pass more information and for future proofing(e.g new ojective types)
	FObjective GetCurrentObjective();
};
