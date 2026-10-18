#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "HealthInterface.generated.h"




// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHealthInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTS_UTILS_API IHealthInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void TakeDamage(AActor* Causer, int32 Index, int32 Damage);

	virtual void TakeDamage(AActor* Causer, int32 Damage);
};
