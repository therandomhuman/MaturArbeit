// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CaptureSystem/CapturePointBase.h"

#include "DefenseMode/CommonGamemodeDataTypes.h"

#include "DefenseGameModeBase.generated.h"

class ACapturePointBase;
class AHUDBase;
class AAICommanderBase;
class AWaveSpawner;

struct FGameplayTag;
/**
 * 
 */

UCLASS()
class RTS_UTILS_API ADefenseGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere,Category = "Points", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACapturePointBase> CapturePointAssigned = nullptr;

	UPROPERTY(EditAnywhere, Category = "Team", meta = (AllowPrivateAccess = "true"))
	FGameplayTag PlayerTeam;

	TArray<AActor*> CapturePoints;

	UPROPERTY(EditAnywhere, Category = "Team", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AAICommanderBase> AiCommander = nullptr;
	UPROPERTY(EditAnywhere, Category = "Team", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWaveSpawner> WaveSpawner = nullptr;

	UFUNCTION()
	void HandlePointCapture(const FGameplayTag& Capturer);




	UPROPERTY()
	TObjectPtr<AHUDBase> ActiveHUD = nullptr;

	FObjective CurrentObjective;
	 
private:
	bool bHasStartedWave = false;
public:

	TObjectPtr<AWaveSpawner> GetWaveSpawner();

	//getter function so that e.g the ai commander knows the current objective, I return a struct so I can pass more information and for future proofing(e.g new ojective types)
	FObjective& GetCurrentObjective();


	void RegisterWaveSpawner(AWaveSpawner* WaveSpawnerToRegister);

	void RegisterAiCommander(AAICommanderBase* AiCommanderToRegister);

	void HandleWaveBeat();
};
