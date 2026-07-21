// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

//inlcluding my interfaces
#include "SelectableInterface.h" // not forward declared because I am deriving the class from this partly
#include "RTS_CommandInterface.h"




//includde generated engine stuff
#include "RTS_Unit_Base.generated.h"

class UCapsuleComponent; // forward decleration to save on compile time and good pratice generally
class UInstancedStaticMeshComponent;
class UFloatingPawnMovement;
class UNavigationSystemV1;

//macro call like Uproperty which sets the metadata and such up for engine use
USTRUCT()//struct to store soldier information
struct FSoldierPropertyStruct
{
	GENERATED_BODY()

	
	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	int32 UnitHealth = 100;

	//Placeholder Variable for storing soldier names. 
	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	FString UnitName = "Max Mustermann";


	//Placeholder Variable for storing soldier type. 
	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	FString UnitType = "Soldier";

	//Variavble to hold relative position in the unit, this isn't an absolute but more of a around here
	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	FVector FormationOffSet;
};


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
	//Declare NavigationSystem
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNavigationSystemV1> NavSys = nullptr;

	//Declare Variable that stores potential FormationOffsets
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Unit,meta = (AllowPrivateAccess = "true"))
	TArray<FVector> DefaultFormationOffsets;

private:

	// Delcaresmth 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> IsmComp = nullptr;
	//declare stat mesh for selection indication
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SelectionBox = nullptr;
	//declare floating pawn movement  for movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMov = nullptr;
	//declare CapsuleComp for Collision
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Collision, meta =(AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComp = nullptr;

	//declare Array to store Soldier Data, I put it into private because I will use getter and setter functions to sanitize data requests insteadd of direct pulic access
	UPROPERTY(VisibleAnywhere, Category = Soldiers, meta = (AllowPrivateAccess = "true"))
	TArray<FSoldierPropertyStruct> SoldierProperties;
	

public:	




	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void OnConstruction(const FTransform& Transform) override;

	void Move();


	void SelectActor_Implementation(const bool Select)override;
	void MoveToLocation_Implementation(const FVector TargetLocation)override;
};
