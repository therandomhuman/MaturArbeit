

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ControllerPawn.generated.h"
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CAMERAUTIL_API AControllerPawn : public APawn
{
	GENERATED_BODY()

	// BaseComponents for the Pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Collision", meta = (AllowPrivateAccess = "true"))

	UCapsuleComponent* CapsuleComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))

	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))

	UCameraComponent* Camera = nullptr;

public:
	// Sets default values for this pawn's properties
	AControllerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
