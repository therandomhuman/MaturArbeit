
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ControllerPawn.generated.h"
class UCapsuleComponent; //forward declerations to save on compile time
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UFloatingPawnMovement;
struct FInputActionValue;
UCLASS()
class CAMERAUTIL_API AControllerPawn : public APawn
{
	GENERATED_BODY()

	// BaseComponents for the Pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Collision", meta = (AllowPrivateAccess = "true"))

	TObjectPtr<UCapsuleComponent> CapsuleComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))

	TObjectPtr<USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))

	TObjectPtr<UCameraComponent> Camera = nullptr;

	//camera properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))

	float ZoomMult = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))

	float ZoomMax = 2500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))

	float ZoomMin = 300.f;

	//declare action for input handling
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ZoomAction = nullptr;
	//declare movement comp

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;
public:
	// Sets default values for this pawn's properties
	AControllerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//declare functions for inputs to fire
	void Move(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
