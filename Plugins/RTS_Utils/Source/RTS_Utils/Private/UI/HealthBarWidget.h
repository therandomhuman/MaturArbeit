// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"




class UBorder;


/**
 * 
 */
UCLASS(abstract)
class UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadOnly,Category = "Border", Meta = (BindWidget))
	TObjectPtr<UBorder> MainBorder = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Border", Meta = (BindWidget))
	TObjectPtr<UBorder> PrecentEmpty = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Border", Meta = (BindWidget))
	TObjectPtr<UBorder> PrecentFilled = nullptr;

	virtual void NativeOnInitialized() override;


#if WITH_EDITOR //just so that this only happens if it's within an editor build, else it might cause build errors

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


private:
	
	UPROPERTY(EditAnywhere, Category = "Internals",
		meta = (ClampMin=0,UIMin=0,ClampMax=1,UIMax=1,Units="Percent"))
	float CurrentPrecentage = 0.f;

	UPROPERTY(EditAnywhere, Category = "Internals",
		meta = (ClampMin = 0, UIMin = 0))
	float CurrentValue = 0.f;

public:
	UFUNCTION()
	void OnStatUpdated(int32 OldValue, int32 NewValue, int32 MaxValue);

	void UpdateWidget();
};
