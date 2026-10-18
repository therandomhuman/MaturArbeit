// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagAssetInterface.h"

#include "RTSPlayerController.generated.h"



/**
 * 
 */
class AHUDBase;
struct FInputActionValue;
class ARTS_Unit_Base;
class UInputAction;
class UInputMappingContext; //forward declaration to save on compile time and generally just good practice
UCLASS()
class CAMERAUTIL_API ARTSPlayerController : public APlayerController
{
	// no keyword means it's public
	GENERATED_BODY() // UE makro for setting up stuff to work with the engine functions
	
	ARTSPlayerController(); // constructor function

	//define reference to inputmappingcontext
	UPROPERTY(EditAnywhere,Category = "Input",meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SelectAction;

	//declare ptr to command action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CommandAction;

	UPROPERTY()

	TObjectPtr<AActor> SelectedActor = nullptr;

	UPROPERTY()
	FGameplayTagContainer SelectedActorTags;

	UPROPERTY()
	TObjectPtr<AHUDBase> ActiveHUD = nullptr;


	virtual void BeginPlay() override;
protected: //only the class itself and children can access protected things

	virtual void SetupInputComponent() override; //virtual void overwrites inherited functions from parent classes

	void SelectHandler(const FInputActionValue& value);

	void CommandHandler(const FInputActionValue& value);

};
