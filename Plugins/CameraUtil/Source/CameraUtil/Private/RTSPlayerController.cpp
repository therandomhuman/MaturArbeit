// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "RTS_Unit_Base.h"

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



	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		//bind input to declared functions
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Completed, this, &ARTSPlayerController::SelectHandler);
		EnhancedInputComponent->BindAction(CommandAction, ETriggerEvent::Completed, this, &ARTSPlayerController::CommandHandler);

	}
}

void ARTSPlayerController::SelectHandler(const FInputActionValue& value)
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, HitResult);
	UE_LOG(LogTemp, Display, TEXT("Select Action"));

	if (SelectedActor)
	{
		if (SelectedActor->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
		{
			ISelectableInterface::Execute_SelectActor(SelectedActor, false);
		}
	}
	SelectedActor = HitResult.GetActor();
	if (SelectedActor)
	{
	

		if (SelectedActor->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
		{ 
			ISelectableInterface::Execute_SelectActor(SelectedActor, true);
		}

	}
}

void ARTSPlayerController::CommandHandler(const FInputActionValue& value)
{
	if (!SelectedActor)
	{
		return;
	}

	if (SelectedActor->GetClass()->ImplementsInterface(URTS_CommandInterface::StaticClass()))
	{ 
		FHitResult HitResult;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, HitResult);
		if (!HitResult.bBlockingHit) //check if the raycast hit smth, I prefer to do them like this to prevent nesting my code too much
		{
			return;
		}
		IRTS_CommandInterface::Execute_MoveToLocation(SelectedActor, HitResult.Location);
	}
}

