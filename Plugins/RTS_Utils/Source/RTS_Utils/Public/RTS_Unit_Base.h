// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

//inlcluding my interfaces
#include "SelectableInterface.h" // not forward declared because I am deriving the class from this partly
#include "RTS_CommandInterface.h"
#include "GameplayTagAssetInterface.h"



//includde generated engine stuff
#include "RTS_Unit_Base.generated.h"

class UCapsuleComponent; // forward decleration to save on compile time and good pratice generally
class UInstancedStaticMeshComponent;
class UFloatingPawnMovement;
class UNavigationSystemV1;
class UNavigationQueryFilter;
class UPathFollowingComponent;
class AAIController;
class UGunHandler;
class UWeaponData;


//macro call like Uproperty which sets the metadata and such up for engine use

struct FPathfindingModification;

struct FSoldierPropertyStruct;

struct FPathFollowingResult;
struct FAIRequestID;

enum class EUnitType;
UCLASS()
class RTS_UTILS_API ARTS_Unit_Base : public APawn, public ISelectableInterface, public IRTS_CommandInterface, public IGameplayTagAssetInterface
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


	//stuff for atttackng

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AttackTarget, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> TargetActor = nullptr;

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



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayTags, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer GameplayTags;





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


	//declare comp that handles gun logic
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGunHandler> GunHandler = nullptr;

	//declare Array to store Soldier Data
	UPROPERTY(EditAnywhere, Category = Soldiers, meta = (AllowPrivateAccess = "true"))
	TArray<FSoldierPropertyStruct> SoldierProperties;

	
	
	//Function to bind to delegate to clean up after movement is done
	void HandleMovementCleanUp(FAIRequestID, const FPathFollowingResult&);


	//Function to bind to delegate to clean up after movement is done
	void RunAttackCheck();



public:	

	//Delcare ISM for weapons
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> ISMWeaponSoldier = nullptr;

	//declare slots for default weapons
	UPROPERTY(EditDefaultsOnly, Category = "Unit|Equipment", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWeaponData> SoldierWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Unit|Equipment", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWeaponData> OfficerWeapon = nullptr;
	//this one only gets used if the officer has a seperate weapon from the soldiers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> ISMWeaponOfficer = nullptr;

	//declare CapsuleComp for Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Unit, meta = (AllowPrivateAccess = "true"))
	EUnitType UnitType;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Unit, meta = (AllowPrivateAccess = "true"))
	int32 test = 1;

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
	const TArray<FSoldierPropertyStruct> GetUnitdata();


	//Getter Function for getting a specific soldier's data, this requires the caller to know the index
	const FSoldierPropertyStruct* GetSoldierData(int32 Index);
	//Setter Function for setting a specific soldier's data, this requires the caller to know the index
	void  SetSoldierData(int32 Index, FSoldierPropertyStruct);
	
	//Declare Interface Implementation
	void SelectActor_Implementation(const bool Select)override;

	void MoveToLocation_Implementation(const FVector TargetLocation)override;

	void AttackObject_Implementation(AActor* NewTargetActor)override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	
};
