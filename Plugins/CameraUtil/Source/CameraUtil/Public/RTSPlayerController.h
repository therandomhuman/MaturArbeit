// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.generated.h"



/**
 * 
 */


class UInputMappingContext; //forward declaration to save on compile time and generally just good practice
UCLASS()
class CAMERAUTIL_API ARTSPlayerController : public APlayerController
{
	// no keyword means it's public
	GENERATED_BODY() // UE makro for setting up stuff to work with the engine functions
	
	ARTSPlayerController(); // constructor function

	//define reference to inputmappingcontext
	UPROPERTY(EditAnywhere,Category = "Input",meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultInputMappingContext;
protected: //only the class itself and children can access protected things

	virtual void SetupInputComponent() override; //virtual void overwrites inherited functions from parent classes

};
