// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStateScreenBase.generated.h"


class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class RTS_UTILS_API UGameStateScreenBase : public UUserWidget
{
	GENERATED_BODY()
	


protected:



	UPROPERTY(BlueprintReadOnly, Category = "Selection", Meta = (BindWidget))
	TObjectPtr<UTextBlock> StateMessage = nullptr;


	UPROPERTY(BlueprintReadOnly, Category = "Selection", Meta = (BindWidget))
	TObjectPtr<UButton> MenuButton = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Selection", Meta = (BindWidget))
	TObjectPtr<UButton> RestartButton = nullptr;
	
	UFUNCTION()
	void ProccessMainMenuButtonClick();
	UFUNCTION()
	void ProccessRetryButtonClick();

public:
	void SetStateText(FText Text);

	void SetUpButtonDelegates();
};
