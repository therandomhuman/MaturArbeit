// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionWidget.generated.h"


class UHorizontalBox;
class UTextBlock;
class ARTS_Unit_Base;
class UImage;
class UHealthBarWidget;
/**
 * 
 */
UCLASS(Abstract)
class RTS_UTILS_API USelectionWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadOnly, Category = "Selection", Meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HealthBarContainer = nullptr;


	UPROPERTY(BlueprintReadOnly, Category = "Selection", Meta = (BindWidget))
	TObjectPtr<UTextBlock> UnitInformation = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Selection", Meta = (BindWidget))
	TObjectPtr<UImage> UnitImage = nullptr;

	// I store them inside a tarray since it's eaiser than a seperate reference for each 
	UPROPERTY(BlueprintReadOnly, Category = "Selection")
	TArray<UHealthBarWidget*> HealthBarWidgets;


public:
	UFUNCTION()
	void UpdateEntireUI(ARTS_Unit_Base* Unit);

	void UpdateHealth(int32 index, int32 Health);
};
