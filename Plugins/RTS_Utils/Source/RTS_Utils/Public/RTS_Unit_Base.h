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
class UNavigationQueryFilter;
class UPathFollowingComponent;
class AAIController;

//macro call like Uproperty which sets the metadata and such up for engine use

USTRUCT()//struct to store pathfinding modifications for soldiers
struct FPathfindingModification
{
	GENERATED_BODY()
	FVector AvoidanceModification = FVector::ZeroVector;

};

USTRUCT()//struct to store soldier information
struct FSoldierPropertyStruct
{
	GENERATED_BODY()

	
	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	int32 UnitHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	int32 Index = 0;

	//Placeholder Variable for storing soldier names. 
	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	FString UnitName = "Max Mustermann";


	//Placeholder Variable for storing soldier type. Will switch to an enum later
	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	FString UnitType = "Soldier";

	//Variavble to hold relative position in the unit, this isn't an absolute but more of a around here
	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	FVector FormationOffSet;

	UPROPERTY(VisibleAnywhere, Category = Soldier, meta = (AllowPrivateAccess = "true"))
	FPathfindingModification PathMods;
};

struct FPathFollowingResult;
struct FAIRequestID;

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
	TArray<FVector> DefaultFormationOffsets = TArray<FVector>();

	//Declare NavQueryFilter
	UPROPERTY(EditDefaultsOnly, Category = Unit, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UNavigationQueryFilter> NavigationQuery = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Debug, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> DebugPath = nullptr;


	//Declare Variables to hold relevant Ai navigation components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Debug, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPathFollowingComponent> PathFollowingComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Debug, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AAIController> AIController = nullptr;



private:

	// Declare ISM
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> IsmComp = nullptr;
	//declare stat mesh for selection indication
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SelectionBox = nullptr;
	//declare floating pawn movement  for movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMov = nullptr;

	//declare Array to store Soldier Data
	UPROPERTY(VisibleAnywhere, Category = Soldiers, meta = (AllowPrivateAccess = "true"))
	TArray<FSoldierPropertyStruct> SoldierProperties;
	
	//Function to bind to delegate to clean up after movement is done
	void HandleMovementCleanUp(FAIRequestID, const FPathFollowingResult&);


public:	



	//declare CapsuleComp for Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComp = nullptr;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void OnConstruction(const FTransform& Transform) override;

	//Hanldes custom per tick movement logic
	void Move(float DeltaTime);


	//Getter Function for the current SharedNavPtr
	const UPathFollowingComponent& GetPathFollowingComp();

	const TObjectPtr<UInstancedStaticMeshComponent> GetIsm();
	//Getter Function to get the Tarray of the entire unit, this is non specific, used in e.g the movement module since that needs them all. 
	const TArray<FSoldierPropertyStruct>& GetUnitdata();


	//Getter Function for getting a specific soldier's data, this requires the caller to know the index
	const FSoldierPropertyStruct* GetSoldierData(int32 Index);
	//Setter Function for setting a specific soldier's data, this requires the caller to know the index
	void  SetSoldierData(int32 Index, FSoldierPropertyStruct);
	
	//Declare Interface Implementation
	void SelectActor_Implementation(const bool Select)override;
	void MoveToLocation_Implementation(const FVector TargetLocation)override;
};
