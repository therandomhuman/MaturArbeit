// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseMode/DefenseGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUDBase.h"


void ADefenseGameModeBase::BeginPlay()
{
    Super::BeginPlay();

 

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

FObjective ADefenseGameModeBase::GetCurrentObjective()
{
    return CurrentObjective;
}
