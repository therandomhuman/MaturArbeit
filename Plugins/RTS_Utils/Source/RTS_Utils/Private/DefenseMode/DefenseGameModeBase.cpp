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
    //get commander
    AActor* CommanderActor = UGameplayStatics::GetActorOfClass(
        GetWorld(),
        AAICommanderBase::StaticClass()
    );
    AiCommander = Cast<AAICommanderBase>(CommanderActor);

    //get wavespawner
    AActor* WaveActor = UGameplayStatics::GetActorOfClass(
        GetWorld(),
        AWaveSpawner::StaticClass()
    );
    WaveSpawner = Cast<AWaveSpawner>(WaveActor);
    WaveSpawner->StartUpWaves();
    AiCommander->SetWaveSpawner(WaveSpawner);

    //just call wavespawner to set up wave system, it waits etc and spawn waves on it's own, it registers them etc....
    //
    ActiveHUD = Cast<AHUDBase>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());



}

void ADefenseGameModeBase::HandlePointCapture(const FGameplayTag& Capturer)
{
    if (PlayerTeam == Capturer)
    {
        return;
    }

    ActiveHUD->ToggleGameStateScreen();
    //give signal to the ui to show end screen 
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}

TObjectPtr<AWaveSpawner> ADefenseGameModeBase::GetWaveSpawner()
{
    return WaveSpawner;
}

FObjective ADefenseGameModeBase::GetCurrentObjective()
{
    return CurrentObjective;
}
