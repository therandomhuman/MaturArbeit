// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

//inlcluding my interfaces
#include "SelectableInterface.h" // not forward declared because I am deriveing the class from this partly
#include "RTS_CommandInterface.h"
#include "RTS_Unit_Base.generated.h"

class UCapsuleComponent; // forward decleration to save on compile time and good pratice generally
class USkeletalMeshComponent;
class UFloatingPawnMovement;
UCLASS()
class RTS_UTILS_API ARTS_Unit_Base : public APawn, public ISelectableInterface, public IRTS_CommandInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTS_Unit_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Navigation stuff
	FVector MoveTargetLocation = FVector::ZeroVector;
	bool bMoving = false;
	float MoveTolerance = 50.f;
private:

	//declare skeletalmesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh = nullptr;
	//declare stat mesh for selection indication
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SelectionBox = nullptr;
	//declare floating pawn movement  for movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMov = nullptr;
	//declare CapsuleComp for Collision
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Collision, meta =(AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComp = nullptr;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move();

	void SelectActor_Implementation(const bool Select)override;
	void MoveToLocation_Implementation(const FVector TargetLocation)override;
};
