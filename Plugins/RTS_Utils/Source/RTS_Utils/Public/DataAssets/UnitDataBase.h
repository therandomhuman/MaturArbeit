

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"


#include "UnitDataBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RTS_UTILS_API UUnitDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:



	UPROPERTY(EditDefaultsOnly)
	FText UnitInformation = FText::FromString(TEXT("Placeholder"));
	
	UPROPERTY(EditDefaultsOnly)
	int32 Range = 100;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> UnitImage = nullptr;


};
