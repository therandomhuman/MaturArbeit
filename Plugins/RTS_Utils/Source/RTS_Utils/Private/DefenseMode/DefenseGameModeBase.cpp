// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseMode/DefenseGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DefenseMode/AICommanderBase.h"
#include "DefenseMode/WaveSpawner.h"
#include "UI/HUDBase.h"


void ADefenseGameModeBase::BeginPlay()
{
    Super::BeginPlay();

 
    //collect all capturepoints
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        ACapturePointBase::StaticClass(),
        CapturePoints
    );

    for (AActor* Actor : CapturePoints)
    {
        ACapturePointBase* CapturePoint = Cast<ACapturePointBase>(Actor);

        if (CapturePoint)
        {
            CurrentObjective.RelevantObject = Actor;
            CapturePoint->CaptureDelegate.AddUObject(this,&ADefenseGameModeBase::HandlePointCapture);
       
        }
    }
    //adapt architucrut
    //get commander
    AActor* CommanderActor = UGameplayStatics::GetActorOfClass(
        GetWorld(),
        AAICommanderBase::StaticClass()
    );
    AiCommander = Cast<AAICommanderBase>(CommanderActor);

 
   



    ActiveHUD = Cast<AHUDBase>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());



}

void ADefenseGameModeBase::HandlePointCapture(const FGameplayTag& Capturer)
{
    if (PlayerTeam == Capturer)
    {
        return;
    }

    ActiveHUD->ToggleGameStateScreen(false);
    //give signal to the ui to show end screen 
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ADefenseGameModeBase::HandleWaveBeat()
{
    ActiveHUD->ToggleGameStateScreen(true);
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}

TObjectPtr<AWaveSpawner> ADefenseGameModeBase::GetWaveSpawner()
{
    return WaveSpawner;
}

FObjective& ADefenseGameModeBase::GetCurrentObjective()
{
    return CurrentObjective;
}

void ADefenseGameModeBase::RegisterWaveSpawner(AWaveSpawner* WaveSpawnerToRegister)
{
    WaveSpawner = WaveSpawnerToRegister;


    //since I don't know which component registers first I check in both registrations for the other one to do wave setup
    if (!bHasStartedWave && AiCommander)
    {
        bHasStartedWave = true;
        AiCommander->SetWaveSpawner(WaveSpawner);
        WaveSpawner->StartUpWaves();
    }
 
}

void ADefenseGameModeBase::RegisterAiCommander(AAICommanderBase* AiCommanderToRegister)
{

    AiCommander = AiCommanderToRegister;

    if (!bHasStartedWave && WaveSpawner)
    {
        bHasStartedWave = true;
        AiCommander->SetWaveSpawner(WaveSpawner);
        WaveSpawner->StartUpWaves();
    }
}
