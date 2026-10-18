// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/HUD.h"
#include "HUDBase.generated.h"

/**
* 
* 
 * 
 */

class ARTS_Unit_Base;
class UGeneralWidget;
class UGameStateScreenBase;
UCLASS()
class RTS_UTILS_API AHUDBase : public AHUD
{
public:
	UPROPERTY(EditAnywhere, category = "UI Classes")
	TSubclassOf<UUserWidget> MainUIWidgetClass = nullptr;

	UPROPERTY(EditAnywhere,category = "UI Classes")
	TSubclassOf<UUserWidget> LoseScreenUIWidgetClass = nullptr;


	void AssignUnit(AActor* NewSelected);

	void ToggleGameStateScreen(bool bWon);
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReasn) override;

private:


	// whenever we change the view mode, this private function is called to show the appropriate widgets.
	void UpdateWidgets();

	// Release any delegate bindings.
	void ClearAllHandlers();

	UPROPERTY()
	TObjectPtr<UWorld> World = nullptr;

	UPROPERTY()
	TObjectPtr<UGeneralWidget>  MainWidget = nullptr;


	UPROPERTY()
	TObjectPtr<UGameStateScreenBase>  LoseScreenWidget = nullptr;


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ARTS_Unit_Base> SelectedUnit = nullptr;


	GENERATED_BODY()
	
};
