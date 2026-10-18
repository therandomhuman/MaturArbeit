

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"




class UBoxComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class UNiagaraComponent;

#include "BaseProjectile.generated.h"

UCLASS()
class RTS_UTILS_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,Category = "Visuals|Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxCollider = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Visuals|Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Visuals|VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> NiagaraSystem = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Hit", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType = nullptr;


	UPROPERTY(VisibleAnywhere, Category = "Hit", meta = (AllowPrivateAccess = "true"))
	int32 BulletDamage = 0;


	//overlap function to bind
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);


	virtual void BulletHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, const FHitResult& SweepRes);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetUpNewProjectile(float StartVelocity, float Gravity, int32 Damage);
};
