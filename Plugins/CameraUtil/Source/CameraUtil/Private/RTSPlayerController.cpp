// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"

ARTSPlayerController::ARTSPlayerController()
{

	bShowMouseCursor = true;

}
void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent(); //calls the parent function this one overwrites

	if (!DefaultInputMappingContext)//if I didn't assign a context return to avoid errors
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* SubSystemLocalPlr = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!SubSystemLocalPlr)
	{
		return;
	}
	SubSystemLocalPlr->AddMappingContext(DefaultInputMappingContext, 0);
}
