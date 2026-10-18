// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameStateScreenBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UGameStateScreenBase::ProccessMainMenuButtonClick()
{
	UE_LOG(LogTemp,Warning,TEXT("Pressed Main Menu"))
		UGameplayStatics::OpenLevel(GetWorld(),FName(*FString("MainMenu")));
}

void UGameStateScreenBase::ProccessRetryButtonClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Pressed Retry"))
		UGameplayStatics::OpenLevel(GetWorld(), *UGameplayStatics::GetCurrentLevelName(GetWorld(), true));
}

void UGameStateScreenBase::SetStateText(FText Text)
{

	StateMessage->SetText(Text);
}

void UGameStateScreenBase::SetUpButtonDelegates()
{
	MenuButton->OnClicked.AddDynamic(this, &UGameStateScreenBase::ProccessMainMenuButtonClick);
	RestartButton->OnClicked.AddDynamic(this, &UGameStateScreenBase::ProccessRetryButtonClick);
}

