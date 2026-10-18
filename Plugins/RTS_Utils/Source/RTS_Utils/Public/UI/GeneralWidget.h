// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GeneralWidget.generated.h"

class USelectionWidget;
class ARTS_Unit_Base;
/**
 * 
 */
UCLASS(Abstract)
class RTS_UTILS_API UGeneralWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadOnly, Category = "Selection", Meta = (BindWidget))
	TObjectPtr<USelectionWidget> SelectedUnitWidget = nullptr;


public:
	UFUNCTION()
	void UpdateEntireUI(ARTS_Unit_Base* Unit);
};
