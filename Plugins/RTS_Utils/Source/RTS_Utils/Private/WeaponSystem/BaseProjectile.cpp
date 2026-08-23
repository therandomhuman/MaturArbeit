


#include "WeaponSystem/BaseProjectile.h"


#include "NiagaraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"


#include "Utilities/HealthInterface.h"

#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("COLLIDER"));
	SetRootComponent(BoxCollider);


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	NiagaraSystem->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	//set up delegate connections for collision
	
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::BeginOverlap);
}

void ABaseProjectile::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor == GetInstigator() || OtherActor == this || !OtherActor)
	{
		return;
	}

	BulletHit(OtherComponent,OtherActor,SweepResult);
	Destroy();


}

void ABaseProjectile::BulletHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, const FHitResult& SweepRes)
{
	AController* Controller = GetInstigator()->GetController();


	IHealthInterface* OtherActorHealthInt = Cast<IHealthInterface>(OtherActor);

	if (OtherActorHealthInt)
	{ 
		//if the hit component is an ism, call the ism specific function(used for units)
		UE_LOG(LogTemp, Warning, TEXT("An Actor's name is %s"), *OverlappedComponent->GetName());
		if (Cast<UInstancedStaticMeshComponent>(OverlappedComponent))
		{
			OtherActorHealthInt->TakeDamage(GetInstigator(), SweepRes.Item, BulletDamage);
		}
		else//if not then use the general take damage 
		{
			OtherActorHealthInt->TakeDamage(GetInstigator(), BulletDamage);
		}
		
	}

}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::SetUpNewProjectile(float StartVelocity, float Gravity, int32 Damage)
{

	ProjectileMovementComponent->Velocity = ProjectileMovementComponent->Velocity.GetSafeNormal()*StartVelocity;
	ProjectileMovementComponent->ProjectileGravityScale = Gravity;
	BulletDamage = Damage;
}


